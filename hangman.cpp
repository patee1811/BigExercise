#include<bits/stdc++.h>
using namespace std;

const string WORD_LIST[] = {
        "angle", "ant", "apple", "arch", "arm", "army",
        "baby", "bag", "ball", "band", "basin", "basket", "bath", "bed", "bee", "bell", "berry",
        "bird", "blade", "board", "boat", "bone", "book", "boot", "bottle", "box", "boy",
        "brain", "brake", "branch", "brick", "bridge", "brush", "bucket", "bulb", "button",
        "cake", "camera", "card",  "cart", "carriage", "cat", "chain", "cheese", "chest",
        "chin", "church", "circle", "clock", "cloud", "coat", "collar", "comb", "cord",
        "cow", "cup", "curtain", "cushion",
        "dog", "door", "drain", "drawer", "dress", "drop", "ear", "egg", "engine", "eye",
        "face", "farm", "feather", "finger", "fish", "flag", "floor", "fly",
        "foot", "fork", "fowl", "frame", "garden", "girl", "glove", "goat", "gun",
        "hair", "hammer", "hand", "hat", "head", "heart", "hook", "horn", "horse",
        "hospital", "house", "island", "jewel",  "kettle", "key", "knee", "knife", "knot",
        "leaf", "leg", "library", "line", "lip", "lock",
        "map", "match", "monkey", "moon", "mouth", "muscle",
        "nail", "neck", "needle", "nerve", "net", "nose", "nut",
        "office", "orange", "oven", "parcel", "pen", "pencil", "picture", "pig", "pin",
        "pipe", "plane", "plate", "plow", "pocket", "pot", "potato", "prison", "pump",
        "rail", "rat", "receipt", "ring", "rod", "roof", "root",
        "sail", "school", "scissors", "screw", "seed", "sheep", "shelf", "ship", "shirt",
        "shoe", "skin", "skirt", "snake", "sock", "spade", "sponge", "spoon", "spring",
        "square", "stamp", "star", "station", "stem", "stick", "stocking", "stomach",
        "store", "street", "sun", "table", "tail", "thread", "throat", "thumb", "ticket",
        "toe", "tongue", "tooth", "town", "train", "tray", "tree", "trousers", "umbrella",
        "wall", "watch", "wheel", "whip", "whistle", "window", "wire", "wing", "worm",
    };

const int WORD_COUNT = sizeof(WORD_LIST) / sizeof(string);
const int MAX_BAD_GUESSES = 7;

string chooseWord();
void renderGame(string guessedWord, int badGuessCount);
char readAGuess();
string update(string guessedWord, char guess, string secretWord);
void displayResult(bool lost);
bool contains(string secretWord, char c);


int main  ()
{
    srand(time(0));
    string secretWord = chooseWord();
    string guessedWord = string(secretWord.length(), '-');
    int badGuessCount = 0;
    do
    {
        renderGame(guessedWord, badGuessCount);
        char guess = readAGuess();
        if (contains(secretWord, guess))
            {
                guessedWord = update(guessedWord, guess, secretWord);
            }
        else badGuessCount++;
    } while (badGuessCount < MAX_BAD_GUESSES && guessedWord != secretWord);

    displayResult(badGuessCount == MAX_BAD_GUESSES);
}


string chooseWord()
{
    int randomIndex = rand() % WORD_COUNT;
    return WORD_LIST[randomIndex];
}
void renderGame(string guessedWord, int badGuessCount)
{
    cout << guessedWord << endl;
    cout << badGuessCount << endl;
}
char readAGuess()
{
    char x;
    cout << "Enter your guess: ";
    cin >> x;
    return x;
}
string update(string guessedWord, char guess, string secretWord)
{
    for (int i = 0; i < guessedWord.length(); i++)
    {
        if (guess == secretWord[i]) guessedWord[i] = guess;
    }
    return guessedWord;
}
void displayResult(bool lost)
{
    if (lost) cout << "You lost";
    else cout << "You won!";
}
bool contains(string word, char c)
{
    return (word.find_first_of(c)!= string::npos);
}
