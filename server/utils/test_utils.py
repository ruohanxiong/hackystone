import config_parser

def test_load_config():
    host = config_parser.get_redis_config('host')
    assert host == 'localhost'

def run():
    test_load_config()

if __name__ == "__main__":
    run()