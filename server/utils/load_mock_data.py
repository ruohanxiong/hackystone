import redis
import config_parser
import csv

r = redis.Redis(host=config_parser.get_redis_config('host'), port=config_parser.get_redis_config('port'), db=0)

def readcsv(file):
    with open(file, 'rb') as csvfile:
        reader = csv.reader(csvfile)
        headers = next(reader, None)
        columns = {}
        for header in headers:
            columns[header] = []
        for row in reader:
            for i in range(0, len(row)):
                columns[headers[i]].append(row[i])
    return columns

def init_measurement_data(file):
    """Inserts tag measurement data from the given file into redis of the current configuration.
    """
    pass

columns = readcsv('../data/mock_data1/anchordata.csv')
print(columns)