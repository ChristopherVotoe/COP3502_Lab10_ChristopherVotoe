#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Trie
{
    int endWord;
    struct Trie *children[26];
};

struct Trie *createNode()
{
    struct Trie *newNode = (struct Trie *)malloc(sizeof(struct Trie));
    newNode->endWord = 0;
    for (int i = 0; i < 26; i++)
    {
        newNode->children[i] = NULL;
    }
    return newNode;
}

void insert(struct Trie *pTrie, char *word)
{
    struct Trie *curr = pTrie;
    for (int i = 0; word[i] != '\0'; i++)
    {
        int index = word[i] - 'a';
        if (curr->children[index] == NULL)
        {
            curr->children[index] = createNode();
        }
        curr = curr->children[index];
    }
    curr->endWord = 1;
}

int numberOfOccurrences(struct Trie *pTrie, char *word)
{
    struct Trie *curr = pTrie;
    int amt = 1;
    for (int i = 0; word[i] != '\0'; i++)
    {
        int index = word[i] - 'a';
        if (curr->children[index] == NULL)
        {
            return 0;
        }
        amt++;
        curr = curr->children[index];
    }
    return amt;
}

struct Trie *deallocateTrie(struct Trie *pTrie)
{
    if (pTrie == NULL)
    {
        return NULL;
    }
    for (int i = 0; i < 26; i++)
    {
        deallocateTrie(pTrie->children[i]);
    }
    free(pTrie);
    return NULL;
}

struct Trie *createTrie()
{
    return createNode();
}

int readDictionary(char *filename, char ***pInWords)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening the file: %s\n", filename);
        return 0;
    }

    int numWords = 0;
    char buffer[256];
    int maxWords = 256;

    *pInWords = (char **)malloc(maxWords * sizeof(char *));
    if (*pInWords == NULL)
    {
        printf("Memory allocation error.\n");
        fclose(file);
        return 0;
    }

    while (fgets(buffer, sizeof(buffer), file))
    {
        buffer[strcspn(buffer, "\n")] = '\0';
        (*pInWords)[numWords] = (char *)malloc(strlen(buffer) + 1);
        if ((*pInWords)[numWords] == NULL)
        {
            printf("Memory allocation error.\n");
            fclose(file);
            return numWords;
        }
        strcpy((*pInWords)[numWords], buffer);
        numWords++;

        if (numWords == maxWords)
        {
            maxWords *= 2;
            *pInWords = (char **)realloc(*pInWords, maxWords * sizeof(char *));
            if (*pInWords == NULL)
            {
                printf("Memory reallocation error.\n");
                fclose(file);
                return numWords;
            }
        }
    }

    fclose(file);
    return numWords;
}

int main(void)
{
    char **inWords = NULL;

    inWords = (char **)malloc(256 * sizeof(char *));
    if (inWords == NULL)
    {
        printf("Memory allocation error.\n");
        return 1;
    }

    int numWords = readDictionary("dictionary.txt", &inWords);
    for (int i = 0; i < numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }

    struct Trie *pTrie = createTrie();
    for (int i = 0; i < numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }

    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    pTrie = deallocateTrie(pTrie);

    for (int i = 0; i < numWords; ++i)
    {
        free(inWords[i]);
    }
    free(inWords);

    return 0;
}
