#### What is the project about?

	This project is a wrapper of Utopian APIs. It provides an easy way to use Utopian APIs for Lua developers.
	The current implementation is a wrapper of Utopian API and the returned values are all in JSON.
	
	Next release will be focus on the output friendly for lua developers:
	1) Convert the JSON string to lua table etc.
	2) Add some utilities function, for example check if given user is a sponsors/moderators

#### Dependence:

	libcurl 7.54, lua5.3

#### Compile:

- windows/msys compiling: gcc -shared utopian.cpp -o utopian.dll -lcurl -llua
- linux compiling: gcc -shared utopian.cpp -o utopian.so -lcurl -llua

#### Install:

	After compile, you will get the utopian.dll on windows, and utopian.so on linux.
	Just put the shared library in the same directory with you lua script, it will works fine.
	
#### How to use it?

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

#### How to contribute?

- Just fork this project, create your branch, commit your changes and send a pull request!

#### How to contact me?

- Just following me on steemit: https://steemit.com/@steemlua

#### Test:

	lua test.lua

#### Test result:

[{"_id":"ancap-ch/from-en","count":384,"rewards":7268.164999999998,"github":{"id":107485935,"name":"fro
m-en","html_url":"https://github.com/ancap-ch/from-en","description":null,"homepage":"","language":null
,"license":{"key":"other","name":"Other","spdx_id":null,"url":null}},"project_url":"https://utopian.io/
project/ancap-ch/from-en/github/107485935/all"},{"_id":"facebook/react","count":335,"rewards":5965.8190
00000003,"github":{"id":10270250,"name":"react","html_url":"https://github.com/facebook/react","descrip
tion":"A declarative, efficient, and flexible JavaScript library for building user interfaces.","homepa
ge":"https://reactjs.org","language":"JavaScript","license":{"key":"mit","name":"MIT License","spdx_id"
:"MIT","url":"https://api.github.com/licenses/mit"}},"project_url":"https://utopian.io/project/facebook
/react/github/10270250/all"}]853