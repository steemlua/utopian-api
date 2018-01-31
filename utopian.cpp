#include <curl/curl.h>
#include <lua.hpp>

extern "C"
{
#define api_url "https://api.utopian.io/api/"

typedef struct MemoryStruct {
  char *memory;
  size_t size;
}data_t;

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
  mem->memory = (char*)realloc(mem->memory, mem->size + realsize + 1);
  if( mem->memory == NULL)
  {
    printf( "No enough memory to allocate\n" );
	return 0;
  }
  
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}

static void easy_init( CURL** hnd, data_t* chunk )
{
  if( hnd == NULL || chunk == NULL ) return;
  
  chunk->memory = (char*)malloc(1);
  chunk->size = 0; 

  *hnd = curl_easy_init();
  curl_easy_setopt(*hnd, CURLOPT_BUFFERSIZE, 1024000L);
  curl_easy_setopt(*hnd, CURLOPT_USERAGENT, "curl/7.54.1");
  curl_easy_setopt(*hnd, CURLOPT_MAXREDIRS, 50L);
  curl_easy_setopt(*hnd, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
  curl_easy_setopt(*hnd, CURLOPT_TCP_KEEPALIVE, 1L);
  curl_easy_setopt(*hnd, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
  curl_easy_setopt(*hnd, CURLOPT_WRITEDATA, (void*)chunk);
  curl_easy_setopt(*hnd, CURLOPT_CONNECTTIMEOUT, 3600); 
  curl_easy_setopt(*hnd, CURLOPT_TIMEOUT, 3600); //timeout in seconds
  //curl_easy_setopt(*hnd, CURLOPT_VERBOSE, 1);
  
  return;
}

static void easy_post( lua_State* l, CURL** hnd, data_t* chunk )
{
  if( hnd == NULL || chunk == NULL ) return;

  if( chunk->memory != NULL )
  {
	lua_pushstring(l, chunk->memory);
	lua_pushinteger(l, chunk->size);
		
	free( chunk->memory );
	chunk->memory = NULL;
	chunk->size = 0;
  }
  else
	printf( "No data received\n" );

  curl_easy_cleanup(*hnd);
  *hnd = NULL;
}

// https://steemit.com/utopian-io/@crokkon/using-the-utopian-api-interface-for-bot-analysis-bisteemit
static int get_sponsors_c( lua_State* l )
{
  CURLcode ret;
  CURL *hnd = NULL;
  data_t chunk;
  easy_init( &hnd, &chunk );

  char jstr[512] = "";
  sprintf( jstr, "%s%s", api_url, "sponsors" );
  int len = strlen(jstr);
  
  curl_easy_setopt(hnd, CURLOPT_URL, jstr);
  ret = curl_easy_perform(hnd);
  
  easy_post( l, &hnd, &chunk );

  return 2;
}

static int get_moderators_c( lua_State* l )
{
  CURLcode ret;
  CURL *hnd = NULL;
  data_t chunk;
  easy_init( &hnd, &chunk );

  char jstr[512] = "";
  sprintf( jstr, "%s%s", api_url, "moderators" );
  int len = strlen(jstr);
  
  curl_easy_setopt(hnd, CURLOPT_URL, jstr);
  ret = curl_easy_perform(hnd);
  
  easy_post( l, &hnd, &chunk );

  return 2;
}

static int get_stats_c( lua_State* l )
{
  CURLcode ret;
  CURL *hnd = NULL;
  data_t chunk;
  easy_init( &hnd, &chunk );

  char jstr[512] = "";
  sprintf( jstr, "%s%s", api_url, "stats" );
  int len = strlen(jstr);
  
  curl_easy_setopt(hnd, CURLOPT_URL, jstr);
  ret = curl_easy_perform(hnd);
  
  easy_post( l, &hnd, &chunk );

  return 2;
}

static int get_posts_c( lua_State* l )
{
  CURLcode ret;
  CURL *hnd = NULL;
  data_t chunk;
  easy_init( &hnd, &chunk );
  
  int limit = lua_tointeger(l, lua_gettop(l));
  lua_pop(l, 1);
  
  const char* section = lua_tostring(l, lua_gettop(l));
  lua_pop(l, 1);
  
  const char* sort = lua_tostring(l, lua_gettop(l));
  lua_pop(l, 1);
  
  const char* filter = lua_tostring(l, lua_gettop(l));
  lua_pop(l, 1);
  
  const char* status = lua_tostring(l, lua_gettop(l));
  lua_pop(l, 1);
  
  const char* type = lua_tostring(l, lua_gettop(l));
  lua_pop(l, 1);

  char jstr[512] = "";
  sprintf( jstr, "%sposts/?limit=%d&section=%s&sortBy=%s&filterBy=%s&status=%s&type=%s", api_url, limit, section, sort, filter, status, type );
  int len = strlen(jstr);
  
  curl_easy_setopt(hnd, CURLOPT_URL, jstr);
  ret = curl_easy_perform(hnd);
  
  easy_post( l, &hnd, &chunk );

  return 2;
}

// https://steemit.com/utopian-io/@samrg472/utopian-io-retrieving-top-projects-with-the-utopian-api
static int get_top_posts_c( lua_State* l )
{
  CURLcode ret;
  CURL *hnd = NULL;
  data_t chunk;
  easy_init( &hnd, &chunk );

  int sort = lua_tointeger(l, lua_gettop(l));
  lua_pop(l, 1);
  
  int limit = lua_tointeger(l, lua_gettop(l));
  lua_pop(l, 1);
  
  const char* stop = lua_tostring(l, lua_gettop(l));
  lua_pop(l, 1);
  
  const char* start = lua_tostring(l, lua_gettop(l));
  lua_pop(l, 1);

  char jstr[1024] = "";
  if( sort == 0 )
	sprintf( jstr, "%sposts/top?limit=%d&start_date=%s&stop_date=%s&sort_by=contributions", api_url, limit, start, stop );
  else
	sprintf( jstr, "%sposts/top?limit=%d&start_date=%s&stop_date=%s&include_rewards=true&sort_by=rewards", api_url, limit, start, stop );
  int len = strlen(jstr);
  
  curl_easy_setopt(hnd, CURLOPT_URL, jstr);
  ret = curl_easy_perform(hnd);
  
  easy_post( l, &hnd, &chunk );

  return 2;
}

/*
 * Registering functions
 */

static const struct luaL_Reg utopianapi[] = {
	// Tags
	{"get_sponsors", get_sponsors_c},
	{"get_moderators", get_moderators_c},
	{"get_stats", get_stats_c},
	{"get_posts", get_posts_c},
	{"get_top_posts", get_top_posts_c},

    {NULL, NULL}
};

int __declspec(dllexport)luaopen_utopian(lua_State* l)
{
    luaL_newlibtable(l, utopianapi);
    luaL_setfuncs(l, utopianapi, 0);
    return 1;
}
} // end extern "C"
