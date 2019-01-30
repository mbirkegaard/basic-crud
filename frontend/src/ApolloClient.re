let inMemoryCache = ApolloInMemoryCache.createInMemoryCache();

let httpLink = ApolloLinks.createHttpLink(~uri=Config.activeConfig.api, ());

let authLink =
  ApolloLinks.createContextLink(() => {
    let authorization =
      switch (Dom.Storage.(getItem("accessToken", localStorage))) {
      | Some(accessToken) => {j|Bearer $accessToken|j}
      | None => ""
      };

    {"authorization": authorization};
  });

let instance =
  ReasonApollo.createApolloClient(
    ~link=ApolloLinks.from([|authLink, httpLink|]),
    ~cache=inMemoryCache,
    (),
  );
