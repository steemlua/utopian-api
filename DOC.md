- require the module:
	
```u = require("utopian")```

- get all sponsors:

```u.get_sponsors()```

- get all moderators:

```u.get_moderators()```

- get current status:

```u.get_stats()```

- get the posts as specified rule:

```u.get_posts("all","any","review","created","all",2)```

- get the top posts as specified rules:

```u.get_top_posts("2018-1-1", "2018-1-31", 10, 0)```