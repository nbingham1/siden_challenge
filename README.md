# Uniqued File Repository

This implements a simple server with the following API:

*HTTP PUT:* save a file to the disk. The file should have a word per line
separated by new lines. The words in the file are uniqued before saving to
disk.

*HTTP GET:* return file the file that is currently stored on disk.

## Running the server

```
./api.py
```

Then send GET or PUT requests to localhost:8080

## using the "uniq" utility

```
cat sample.txt | ./uniq <outfile>
```

