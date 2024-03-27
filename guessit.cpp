#include<bits/stdc++.h>
using namespace std;

int generateRandomNumber();
int getPlayerGuess();
void printAnswer(int guess, int secretNumber);
void playGuessIt();
int main ()
{
    int n;
    while (1)
    {
        playGuessIt();
        cout << "Ban co muon choi nua khong ? " << endl;
        cout << "Neu co hay nhan phim 1 nguoc lai 0." << endl;
        cin >> n;
        if (n == 0) return 0;
    }
}
void playGuessIt()
{
    srand(int(time(0)));
    int score = 0;
    int secretNumber = generateRandomNumber();
    int guess;
    while (1) {
        score++;
        guess = getPlayerGuess();
        printAnswer(guess, secretNumber);
        if (guess == secretNumber) {
            cout << "So lan doan: " << score << endl;
            cout << "Diem cua nguoi choi: " << 100 - score;
            break;
        }
    }
}
int generateRandomNumber()
{
    return rand() % 100 + 1;
}
int getPlayerGuess()
{
    int guess;
    cin >> guess;
    if (!cin.good())
    {
        cout << "input error";
        exit(0);
    }
    return guess;
}
void printAnswer(int guess, int secretNumber)
{
    if (guess == secretNumber)
    {
        cout << "correct";
    }
    else if (guess > secretNumber)
    {
        cout << "bigger" << endl;
    }
    else cout << "smaller" << endl;
}
