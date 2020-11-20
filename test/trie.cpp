#include <gtest/gtest.h>

#include <src/trie.h>

TEST(trie, separate)
{
	trie t;
	EXPECT_EQ(t.insert("hello"), true);
	EXPECT_EQ(t.insert("blarg"), true);

	EXPECT_EQ(t.insert("hello"), false);
	EXPECT_EQ(t.insert("blarg"), false);
	
	EXPECT_EQ(t.has("hello"), true);
	EXPECT_EQ(t.has("blarg"), true);
	EXPECT_EQ(t.has("he"), false);
	EXPECT_EQ(t.has("bla"), false);
	EXPECT_EQ(t.has("blah"), false);
	EXPECT_EQ(t.has("blargle"), false);
	EXPECT_EQ(t.has("blar"), false);
	EXPECT_EQ(t.has("bla"), false);
	EXPECT_EQ(t.has("bl"), false);
	EXPECT_EQ(t.has("b"), false);
	EXPECT_EQ(t.has(""), false);
	EXPECT_EQ(t.has("h"), false);
	EXPECT_EQ(t.has("hel"), false);
}

TEST(trie, substring)
{
	trie t;
	EXPECT_EQ(t.insert("hello"), true);
	EXPECT_EQ(t.insert("he"), true);

	EXPECT_EQ(t.insert("hello"), false);
	EXPECT_EQ(t.insert("he"), false);
	
	EXPECT_EQ(t.has("hello"), true);
	EXPECT_EQ(t.has("hell"), false);
	EXPECT_EQ(t.has("hel"), false);
	EXPECT_EQ(t.has("he"), true);
	EXPECT_EQ(t.has("h"), false);
	EXPECT_EQ(t.has(""), false);
}

TEST(trie, superstring)
{
	trie t;
	EXPECT_EQ(t.insert("he"), true);
	EXPECT_EQ(t.insert("hello"), true);

	EXPECT_EQ(t.insert("he"), false);
	EXPECT_EQ(t.insert("hello"), false);
	
	EXPECT_EQ(t.has("hello"), true);
	EXPECT_EQ(t.has("hell"), false);
	EXPECT_EQ(t.has("hel"), false);
	EXPECT_EQ(t.has("he"), true);
	EXPECT_EQ(t.has("h"), false);
	EXPECT_EQ(t.has(""), false);
}

TEST(trie, branch)
{
	trie t;
	EXPECT_EQ(t.insert("helen"), true);
	EXPECT_EQ(t.insert("hello"), true);
	
	EXPECT_EQ(t.insert("helen"), false);
	EXPECT_EQ(t.insert("hello"), false);

	EXPECT_EQ(t.has("helen"), true);
	EXPECT_EQ(t.has("hello"), true);
	EXPECT_EQ(t.has("hell"), false);
	EXPECT_EQ(t.has("hel"), false);
	EXPECT_EQ(t.has("he"), false);
	EXPECT_EQ(t.has("h"), false);
	EXPECT_EQ(t.has(""), false);
}

TEST(trie, characters)
{
	trie t;
	EXPECT_EQ(t.insert("abcd!@#$$%^&**("), true);
	EXPECT_EQ(t.insert("abcd!@#$$%"), true);
	EXPECT_EQ(t.insert("abcd!@#$$%"), false);
	EXPECT_EQ(t.insert("abcd!@#$$%^&**("), false);
}

TEST(trie, multibranch)
{
	trie t;
	EXPECT_EQ(t.insert("helen"), true);
	EXPECT_EQ(t.insert("help"), true);
	EXPECT_EQ(t.insert("helvetica"), true);
	EXPECT_EQ(t.insert("hello"), true);

	EXPECT_EQ(t.insert("helen"), false);
	EXPECT_EQ(t.insert("help"), false);
	EXPECT_EQ(t.insert("helvetica"), false);
	EXPECT_EQ(t.insert("hello"), false);

	EXPECT_EQ(t.has("helen"), true);
	EXPECT_EQ(t.has("hello"), true);
	EXPECT_EQ(t.has("hell"), false);
	EXPECT_EQ(t.has("hel"), false);
	EXPECT_EQ(t.has("he"), false);
	EXPECT_EQ(t.has("h"), false);
	EXPECT_EQ(t.has(""), false);
}

TEST(trie, branchsub)
{
	trie t;
	EXPECT_EQ(t.insert("helen"), true);
	EXPECT_EQ(t.insert("help"), true);
	EXPECT_EQ(t.insert("hel"), true);

	EXPECT_EQ(t.insert("helen"), false);
	EXPECT_EQ(t.insert("help"), false);
	EXPECT_EQ(t.insert("hel"), false);


	EXPECT_EQ(t.has("helen"), true);
	EXPECT_EQ(t.has("hello"), false);
	EXPECT_EQ(t.has("hell"), false);
	EXPECT_EQ(t.has("hel"), true);
	EXPECT_EQ(t.has("he"), false);
	EXPECT_EQ(t.has("h"), false);
	EXPECT_EQ(t.has(""), false);
}

