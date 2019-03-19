import redis
import config_parser

r = redis.Redis(host=config_parser.get_redis_config('host'), port=config_parser.get_redis_config('port'), db=0)


def init_measurement_data(file):
    """Inserts tag measurement data from the given file into redis of the current configuration.
    """