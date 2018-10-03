from app import app
from sqlalchemy import Column, Boolean, Integer, String, ForeignKey, create_engine
from sqlalchemy.orm import relationship, sessionmaker, scoped_session
from sqlalchemy.ext.declarative import declarative_base

Base = declarative_base()
engine = create_engine(app.config['DB_URI'], connect_args={
                       'check_same_thread': False}, convert_unicode=True)
db_session = scoped_session(sessionmaker(autocommit=False,
                                         autoflush=False,
                                         bind=engine))
Base.query = db_session.query_property()


class Todo(Base):
    __tablename__ = 'location'
    id = Column(Integer, primary_key=True)
    name = Column(String, nullable=False)
    finished = Column(Boolean, nullable=False)
    project_id = Column(Integer, ForeignKey('project.id'))
    project = relationship('Project', back_populates='todos')


class Project(Base):
    __tablename__ = 'project'
    id = Column(Integer, primary_key=True)
    name = Column(String, nullable=False)
    todos = relationship('Todo', back_populates='project',
                         cascade="all, delete, delete-orphan")


def init_db():
    Base.metadata.drop_all(bind=engine)
    Base.metadata.create_all(bind=engine)
