#include <gtest/gtest.h>

#include <src/trie.h>

TEST(trie, separate)
{
	trie<bool> t;
	EXPECT_EQ(t.insert("hello", true), true);
	EXPECT_EQ(t.insert("blarg", true), true);

	EXPECT_EQ(t.insert("hello", true), false);
	EXPECT_EQ(t.insert("blarg", true), false);
	
	EXPECT_EQ(t.get("hello") != nullptr, true);
	EXPECT_EQ(t.get("blarg") != nullptr, true);
	EXPECT_EQ(t.get("he") != nullptr, false);
	EXPECT_EQ(t.get("bla") != nullptr, false);
	EXPECT_EQ(t.get("blah") != nullptr, false);
	EXPECT_EQ(t.get("blargle") != nullptr, false);
	EXPECT_EQ(t.get("blar") != nullptr, false);
	EXPECT_EQ(t.get("bla") != nullptr, false);
	EXPECT_EQ(t.get("bl") != nullptr, false);
	EXPECT_EQ(t.get("b") != nullptr, false);
	EXPECT_EQ(t.get("") != nullptr, false);
	EXPECT_EQ(t.get("h") != nullptr, false);
	EXPECT_EQ(t.get("hel") != nullptr, false);
}

TEST(trie, substring)
{
	trie<bool> t;
	EXPECT_EQ(t.insert("hello", true), true);
	EXPECT_EQ(t.insert("he", true), true);

	EXPECT_EQ(t.insert("hello", true), false);
	EXPECT_EQ(t.insert("he", true), false);
	
	EXPECT_EQ(t.get("hello") != nullptr, true);
	EXPECT_EQ(t.get("hell") != nullptr, false);
	EXPECT_EQ(t.get("hel") != nullptr, false);
	EXPECT_EQ(t.get("he") != nullptr, true);
	EXPECT_EQ(t.get("h") != nullptr, false);
	EXPECT_EQ(t.get("") != nullptr, false);
}

TEST(trie, superstring)
{
	trie<bool> t;
	EXPECT_EQ(t.insert("he", true), true);
	EXPECT_EQ(t.insert("hello", true), true);

	EXPECT_EQ(t.insert("he", true), false);
	EXPECT_EQ(t.insert("hello", true), false);
	
	EXPECT_EQ(t.get("hello") != nullptr, true);
	EXPECT_EQ(t.get("hell") != nullptr, false);
	EXPECT_EQ(t.get("hel") != nullptr, false);
	EXPECT_EQ(t.get("he") != nullptr, true);
	EXPECT_EQ(t.get("h") != nullptr, false);
	EXPECT_EQ(t.get("") != nullptr, false);
}

TEST(trie, branch)
{
	trie<bool> t;
	EXPECT_EQ(t.insert("helen", true), true);
	EXPECT_EQ(t.insert("hello", true), true);
	
	EXPECT_EQ(t.insert("helen", true), false);
	EXPECT_EQ(t.insert("hello", true), false);

	EXPECT_EQ(t.get("helen") != nullptr, true);
	EXPECT_EQ(t.get("hello") != nullptr, true);
	EXPECT_EQ(t.get("hell") != nullptr, false);
	EXPECT_EQ(t.get("hel") != nullptr, false);
	EXPECT_EQ(t.get("he") != nullptr, false);
	EXPECT_EQ(t.get("h") != nullptr, false);
	EXPECT_EQ(t.get("") != nullptr, false);
}

TEST(trie, characters)
{
	trie<bool> t;
	EXPECT_EQ(t.insert("abcd!@#$$%^&**(", true), true);
	EXPECT_EQ(t.insert("abcd!@#$$%", true), true);
	EXPECT_EQ(t.insert("abcd!@#$$%", true), false);
	EXPECT_EQ(t.insert("abcd!@#$$%^&**(", true), false);
}

TEST(trie, multibranch)
{
	trie<bool> t;
	EXPECT_EQ(t.insert("helen", true), true);
	EXPECT_EQ(t.insert("help", true), true);
	EXPECT_EQ(t.insert("helvetica", true), true);
	EXPECT_EQ(t.insert("hello", true), true);

	EXPECT_EQ(t.insert("helen", true), false);
	EXPECT_EQ(t.insert("help", true), false);
	EXPECT_EQ(t.insert("helvetica", true), false);
	EXPECT_EQ(t.insert("hello", true), false);

	EXPECT_EQ(t.get("helen") != nullptr, true);
	EXPECT_EQ(t.get("hello") != nullptr, true);
	EXPECT_EQ(t.get("hell") != nullptr, false);
	EXPECT_EQ(t.get("hel") != nullptr, false);
	EXPECT_EQ(t.get("he") != nullptr, false);
	EXPECT_EQ(t.get("h") != nullptr, false);
	EXPECT_EQ(t.get("") != nullptr, false);
}

TEST(trie, branchsub)
{
	trie<bool> t;
	EXPECT_EQ(t.insert("helen", true), true);
	EXPECT_EQ(t.insert("help", true), true);
	EXPECT_EQ(t.insert("hel", true), true);

	EXPECT_EQ(t.insert("helen", true), false);
	EXPECT_EQ(t.insert("help", true), false);
	EXPECT_EQ(t.insert("hel", true), false);


	EXPECT_EQ(t.get("helen") != nullptr, true);
	EXPECT_EQ(t.get("hello") != nullptr, false);
	EXPECT_EQ(t.get("hell") != nullptr, false);
	EXPECT_EQ(t.get("hel") != nullptr, true);
	EXPECT_EQ(t.get("he") != nullptr, false);
	EXPECT_EQ(t.get("h") != nullptr, false);
	EXPECT_EQ(t.get("") != nullptr, false);
}

TEST(trie, write_base)
{
	trie<bool> t;
	EXPECT_EQ(t.insert("helen", true), true);
	EXPECT_EQ(t.insert("help", true), true);
	EXPECT_EQ(t.insert("helvetica", true), true);
	EXPECT_EQ(t.insert("hello", true), true);

	stringstream str;
	str << &t;
	EXPECT_EQ(str.str(), "(hel(lo:1,vetica:1,p:1,en:1))");
}

TEST(trie, read_base)
{
	trie<bool> t;
	string store = "(hel(lo:1,vetica:1,p:1,en:1))";
	stringstream str(store);
	str >> &t;

	EXPECT_EQ(t.get("hello") != nullptr, true);
	EXPECT_EQ(t.get("helvetica") != nullptr, true);
	EXPECT_EQ(t.get("help") != nullptr, true);
	EXPECT_EQ(t.get("helen") != nullptr, true);
	EXPECT_EQ(t.get("hell") != nullptr, false);
	EXPECT_EQ(t.get("hel") != nullptr, false);
	EXPECT_EQ(t.get("he") != nullptr, false);
	EXPECT_EQ(t.get("h") != nullptr, false);
	EXPECT_EQ(t.get("") != nullptr, false);
}


