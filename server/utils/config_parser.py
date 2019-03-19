from ConfigParser import SafeConfigParser

configuration = '../configurations/local.cfg'

parser = SafeConfigParser()
parser.read(configuration)

def get_redis_config(key):
    return parser.get('redis', key)