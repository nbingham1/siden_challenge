all:
	g++ -g src/main.cpp -o uniq	

test_uniq:
	g++ -g -L. -Igoogletest -I. test/gtest_main.cc test/trie.cpp -pthread -lgtest -o test_uniq
