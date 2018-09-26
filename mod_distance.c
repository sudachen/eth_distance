#include <sqlite3.h>
#include <sqlite3ext.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>
#define pi 3.14159265358979323846

SQLITE_EXTENSION_INIT1;
static double DEG_TO_RAD = 0.017453292519943295769236907684886;

static void eth_point(sqlite3_context *ctx, int n, sqlite3_value **args){
    if ( n == 2 ) {
        uint64_t lat = (uint32_t)(((int32_t)(sqlite3_value_double(args[0])*100000))%10000000);
        uint64_t lng = (uint32_t)(((int32_t)(sqlite3_value_double(args[1])*100000))%10000000);
        sqlite3_result_int64(ctx,(lat << 32 | lng));
    } 
}

static void eth_lat(sqlite3_context *ctx, int n, sqlite3_value **args){
    if ( n == 1 ) {
        uint64_t latlng1 = sqlite3_value_int64(args[0]);
        double lat1 = ((double)(int32_t)(latlng1>>32))/100000;
        sqlite3_result_double(ctx,lat1);
    } 
}

static void eth_lng(sqlite3_context *ctx, int n, sqlite3_value **args){
    if ( n == 1 ) {
        uint64_t latlng1 = sqlite3_value_int64(args[0]);
        double lng1 = ((double)(int32_t)(latlng1))/100000;
        sqlite3_result_double(ctx,lng1);
    } 
}

static void eth_distance(sqlite3_context *ctx, int n ,sqlite3_value **args) {
    if ( n == 2) {
        uint64_t latlng1 = sqlite3_value_int64(args[0]);
        double lat1 = ((double)(int32_t)(latlng1>>32))/100000;
        double lng1 = ((double)(int32_t)(latlng1))/100000;
        uint64_t latlng2 = sqlite3_value_int64(args[1]);
        double lat2 = ((double)(int32_t)(latlng2>>32))/100000;
        double lng2 = ((double)(int32_t)(latlng2))/100000;
        double latarc  = (lat1 - lat2) * DEG_TO_RAD;  
        double lngarc = (lng1 - lng2) * DEG_TO_RAD;  
        double lath = sin(latarc * 0.5);  
        lath *= lath;  
        double lngh = sin(lngarc * 0.5);  
        lngh *= lngh;  
        double dist =  2.0 * asin(sqrt(lath+ cos(lat1*DEG_TO_RAD) * cos(lat2*DEG_TO_RAD)*lngh)) * 6372.797560856;  
        sqlite3_result_double(ctx,dist);
    }
}

int sqlite3_extension_init( /* <== Change this name, maybe */
  sqlite3 *db, 
  char **pzErrMsg, 
  const sqlite3_api_routines *pApi
){
  int rc = SQLITE_OK;
  SQLITE_EXTENSION_INIT2(pApi);
  sqlite3_create_function(db, "eth_point", 2, SQLITE_ANY, NULL, eth_point, NULL, NULL);
  sqlite3_create_function(db, "eth_distance", 2, SQLITE_ANY, NULL, eth_distance, NULL, NULL);
  sqlite3_create_function(db, "eth_lat", 1, SQLITE_ANY, NULL, eth_lat, NULL, NULL);
  sqlite3_create_function(db, "eth_lng", 1, SQLITE_ANY, NULL, eth_lng, NULL, NULL);
  return rc;
}


