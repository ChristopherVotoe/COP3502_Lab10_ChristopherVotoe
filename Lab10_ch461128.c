#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie structure
struct Trie
{
    char data;
    int endWord;
    struct Trie *children[26];
};

// Creates a new TrieNode
struct Trie *createNode(char data)
{
    struct Trie *newNode = (struct Trie *)malloc(sizeof(struct Trie));
    newNode->data = data;
    newNode->endWord = 0;

    for (int i = 0; i < 26; i++)
    {
        newNode->children[i] = NULL;
    }

    return newNode;
}

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
    struct Trie *curr = pTrie;

    for (int i = 0; word[i] != '\0'; i++)
    {
        int index = word[i] - 'a';
        if (curr->children[index] == NULL)
        {
            curr->children[index] = createNode(word[i]);
        }

        curr = curr->children[index];
    }

    curr->endWord = 1; // Mark the last node as the end of the word
}

// Computes the number of occurrences of the word
int numberOfOccurrences(struct Trie *pTrie, char *word)
{
    struct Trie *curr = pTrie;
    int amt = 0;
    for (int i = 0; word[i] != '\0'; i++)
    {
        int index = word[i] - 'a';
        if (curr->children[index] == NULL)
        {
            return 0;
        }
        curr->endWord++;
        curr = curr->children[index];
    }

    return curr->endWord;
}

// Deallocates the trie structure
void deallocateTrie(struct Trie *pTrie)
{
    if (pTrie == NULL)
    {
        return;
    }
    for (int i = 0; i < 26; i++)
    {
        deallocateTrie(pTrie->children[i]);
    }
    free(pTrie);
}

// Initializes a trie structure
struct Trie *createTrie()
{
    return createNode('\0'); // Root node has a null character as data
}

// This function will return the number of words in the dictionary,
// and read all the words in the dictionary to the structure words
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
    int maxWords = 256; // Maximum number of words

    *pInWords = (char **)malloc(maxWords * sizeof(char *));
    if (*pInWords == NULL)
    {
        printf("Memory allocation error.\n");
        fclose(file);
        return 0;
    }

    while (fgets(buffer, sizeof(buffer), file))
    {
        // Remove the newline character at the end
        buffer[strcspn(buffer, "\n")] = '\0';

        // Allocate memory for the word
        (*pInWords)[numWords] = (char *)malloc(strlen(buffer) + 1);
        if ((*pInWords)[numWords] == NULL)
        {
            printf("Memory allocation error.\n");
            fclose(file);
            return numWords;
        }

        strcpy((*pInWords)[numWords], buffer);

        numWords++;

        // Resize the array if necessary
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
    char **inWords = NULL; // Pointer to store the array of words

    // Read the number of words in the dictionary
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

    // Parse line by line, and insert each word to the trie data structure
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    // Deallocate the Trie
    deallocateTrie(pTrie);

    // Free the memory for the words in the inWords array
    for (int i = 0; i < numWords; ++i)
    {
        free(inWords[i]);
    }
    free(inWords); // Free the array of words

    return 0;
}
