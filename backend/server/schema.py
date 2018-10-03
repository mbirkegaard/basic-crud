import graphene
from graphene_sqlalchemy import SQLAlchemyObjectType, get_query
from . import models
from graphene import NonNull, Float, String, ID, Boolean


class Todo(SQLAlchemyObjectType):
    class Meta:
        model = models.Todo
        exclude_fields = ("project_id", )

    name = NonNull(String)
    finished = NonNull(Boolean)
    project = NonNull(lambda: Project)

    @staticmethod
    def get_or_raise(id):
        todo = models.Todo.query.get(id)
        if todo is None:
            raise Exception("No Todo with id %s" % id)
        return todo


class Project(SQLAlchemyObjectType):
    class Meta:
        model = models.Project

    finished = NonNull(Boolean)
    todos = NonNull(graphene.List(NonNull(Todo)))

    def resolve_finished(self, info):
        return all(todo.finished for todo in self.todos)

    @staticmethod
    def get_or_raise(id):
        project = models.Project.query.get(id)
        if project is None:
            raise Exception("No Project with id %s" % id)
        return project


class Query(graphene.ObjectType):
    hello_world = graphene.String()

    def resolve_hello_world(self, info):
        return "Hello World!"

    projects = NonNull(graphene.List(NonNull(Project)))

    def resolve_projects(self, info):
        return Project.get_query(info)

    project = graphene.Field(Project, id=NonNull(graphene.ID))

    def resolve_project(self, info, id):
        return Project.get_or_raise(id)


class AddProject(graphene.Mutation):
    class Arguments:
        name = NonNull(String)

    ok = NonNull(graphene.Boolean)
    project = graphene.Field(Project)

    def mutate(self, info, name):
        project = models.Project(name=name)
        models.db_session.add(project)
        models.db_session.commit()
        return AddProject(ok=True, project=project)


class DeleteProject(graphene.Mutation):
    class Arguments:
        id = NonNull(ID)

    ok = NonNull(Boolean)
    project = graphene.Field(Project)

    def mutate(self, info, id):
        project_to_delete = Project.get_or_raise(id)
        models.db_session.delete(project_to_delete)
        models.db_session.commit()
        return DeleteProject(ok=True, project=project_to_delete)


class AddTodo(graphene.Mutation):
    class Arguments:
        project_id = NonNull(ID)
        todo_name = NonNull(String)

    ok = NonNull(Boolean)
    todo = graphene.Field(Todo)

    def mutate(self, info, project_id, todo_name):
        project = Project.get_or_raise(project_id)
        todo = models.Todo(name=todo_name, finished=False)
        project.todos.append(todo)
        models.db_session.commit()
        return AddTodo(ok=True, todo=todo)


class ToggleTodo(graphene.Mutation):
    class Arguments:
        id = NonNull(ID)

    ok = NonNull(Boolean)
    todo = graphene.Field(Todo)

    def mutate(self, info, id):
        todo = Todo.get_or_raise(id)
        todo.finished = not(todo.finished)
        models.db_session.commit()
        return ToggleTodo(ok=True, todo=todo)


class DeleteTodo(graphene.Mutation):
    class Arguments:
        id = NonNull(ID)

    ok = NonNull(Boolean)
    todo = graphene.Field(Todo)

    def mutate(self, info, id):
        todo_to_delete = Todo.get_or_raise(id)
        models.db_session.delete(todo_to_delete)
        models.db_session.commit()
        return DeleteTodo(ok=True, todo=todo_to_delete)


class Mutation(graphene.ObjectType):
    add_project = AddProject.Field(required=True)
    delete_project = DeleteProject.Field(required=True)
    add_todo = AddTodo.Field(required=True)
    toggle_todo = ToggleTodo.Field(required=True)
    delete_todo = DeleteTodo.Field(required=True)


schema = graphene.Schema(query=Query, mutation=Mutation)
