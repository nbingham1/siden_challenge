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
make
cat sample.txt | ./uniq <outfile>
```

Testing:

```
make test_uniq
./test_uniq
```

## Design Decisions

For the server, I decided to go with a simple python server. Python is fairly
easy to work with for socket programming and is well documented. Algorithmic
performance here is not necessary.

For the compute element, I decided to go with a simple C++ program that
implements a trie. C++ was chosen for execution speed, and a trie has extremely
good algorithmic performance and pretty good memory usage. There are only
around 300,000 words in the english language, and many of those words share
prefixes with eachother. Overall, given an average word length of `m`
characters, the insertion and search complexity for a single word is `O(m)`.
For a file with `n` words, the total algorithmic complexity is `O(nm)`. The
worst case memory usage is also `O(nm)`, but the real outcome is much less.

