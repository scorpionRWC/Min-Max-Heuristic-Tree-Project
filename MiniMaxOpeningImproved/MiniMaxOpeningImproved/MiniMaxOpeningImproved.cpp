#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Morris
{
private:
    int numWhitePieces;
    int numBlackPieces;
    int numWhiteMoves;
    int numBlackMoves;
    struct Board
    {
        char boardB[23];
        Board *next;
        Board *head;
        Board *current;
        Board *nextSibling;
        Board *child;
        Board *currentSibling;
    };
    Board *root;
    int neigh[4];
    bool isWhite;
    int leaves;

public:
    Board* GenerateMovesOpening(char[]);
    Board* GenerateMovesMidgameEndgame(char[]);
    Board* GenerateAdd(char[]);
    Board* GenerateHopping(char[]);
    Board* GenerateMove(char[]);
    void GenerateRemove(char[], Board*);
    void neighbors(int);
    bool closeMill(int, char[]);
    int staticEstOpen(char[]);
    int staticEstMidEnd(char[]);
    int staticEstImproved(char[]);
    Board* makeTree(char[], int, Board*, int);
    Board* addNode(Board*, char[]);
    int max(int, int);
    int min(int, int);
    int MaxMin(Board*);
    int MinMax(Board*);
    int ABMaxMin(Board*, int, int);
    int ABMinMax(Board*, int, int);
    void printTree(Board*, int, int);
    void printBoard(char[]);
    void testList(char[], int, string);
};

Morris::Board* Morris::GenerateMovesOpening(char board[])
{
    return GenerateAdd(board);
}

Morris::Board* Morris::GenerateMovesMidgameEndgame(char board[])
{
    numWhitePieces = 0;
    for (int i = 0; i < 23; i++)
    {
        if (board[i] == 'W') numWhitePieces++;
    }
    if (numWhitePieces == 3) return GenerateHopping(board);
    else return GenerateMove(board);
}

Morris::Board* Morris::GenerateAdd(char board[])
{
    char piece;
    if (isWhite == true) piece = 'W';
    else piece = 'B';
    char b[23];
    Board *L = new Board();
    L->head = L;
    L->next = NULL;
    L->current = L->head;
    for (int i = 0; i < 23; i++)
    {
        Board * buffer = new Board();
        if (board[i] == 'x') // for every empty spot on the board
        {
            for (int j = 0; j < 23; j++) // make a copy of the board
            {
                b[j] = board[j];
            }
            b[i] = piece; // fill the empty spot with an AI's Piece
            if (closeMill(i, b)) GenerateRemove(b, L);  // if Mill, then remove opponent's Piece
            else // otherwise...
            {
                for (int k = 0; k < 23; k++)
                {
                    buffer->boardB[k] = b[k]; // add b to L
                                              //  cout << buffer->boardB[k];
                }
                //cout << endl;
                L->current->next = buffer;
                L->current = L->current->next;
                L->current->next = NULL;
            }
        } // end if
    } // end for
    return L;
}

Morris::Board* Morris::GenerateHopping(char board[])
{
    char piece;
    if (isWhite == true) piece = 'W';
    else piece = 'B';
    char b[23];
    Board *L = new Board();
    L->head = L;
    L->next = NULL;
    L->current = L->head;
    for (int i = 0; i < 23; i++)
    {
        if (board[i] == piece) // for every spot on the board containing the AI's Piece
        {
            for (int k = 0; k < 23; k++)
            {
                Board *buffer = new Board();
                if (board[k] == 'x')
                {
                    for (int j = 0; j < 23; j++) // make a copy of the board
                    {
                        b[j] = board[j];
                    }
                    b[i] = 'x'; // take the White Piece off...
                    b[k] = piece; // ... and put it here
                    if (closeMill(k, b)) GenerateRemove(b, L); // if Mill, then remove opponents Piece
                    else // otherwise...
                    {
                        for (int g = 0; g < 23; g++)
                        {
                            buffer->boardB[g] = b[g]; // add b to L
                        }
                        L->current->next = buffer;
                        L->current = L->current->next;
                        L->current->next = NULL;
                    }
                } // end if (board[k]...
            } // end for(int k...
        } // end if (board[i]...
    } // end for(int i...
    return L;
}

Morris::Board* Morris::GenerateMove(char board[])
{
    char piece;
    if (isWhite == true) piece = 'W';
    else piece = 'B';
    char b[23];
    Board *L = new Board();
    L->head = L;
    L->next = NULL;
    L->current = L->head;
    for (int i = 0; i < 23; i++) // for each location on the board
    {
        if (board[i] == piece) // if the location contains the AI's Piece
        {
            neighbors(i); // locate the neighbors
            for (int j = 0; j < 4; j++) // check the neighbors
            {
                Board *buffer = new Board();
                if (neigh[j] != -1 && board[neigh[j]] == 'x') // if a neighbor exists and it is empty
                {
                    for (int k = 0; k < 23; k++) // make a copy of the board
                    {
                        b[k] = board[k];
                    } // end for(int k...
                    b[i] = 'x';
                    b[neigh[j]] = piece; // move the AI's Piece to the empty space
                    if (closeMill(neigh[j], b)) GenerateRemove(b, L);
                    else // otherwise...
                    {
                        for (int g = 0; g < 23; g++)
                        {
                            buffer->boardB[g] = b[g]; // add b to L
                        }
                        L->current->next = buffer;
                        L->current = L->current->next;
                        L->current->next = NULL;
                    }
                } // end if(neigh[j...
            } // end for(int j...
        } // end if(board[i]...
    } // end for(int i...
    return L;
}

void Morris::GenerateRemove(char board[], Morris::Board* L)
{
    char b[23];
    for (int i = 0; i < 23; i++) // for each location on the board
    {
        Board *buffer = new Board();
        if ((board[i] == 'B' && isWhite == true) || (board[i] == 'W' && isWhite == false))
        {
            if (!closeMill(i, board))
            {
                for (int k = 0; k < 23; k++) // make a copy of the board
                {
                    b[k] = board[k];
                } // end for(int k...
                b[i] = 'x';
                for (int g = 0; g < 23; g++)
                {
                    buffer->boardB[g] = b[g]; // add b to L
                }
                L->current->next = buffer;
                L->current = L->current->next;
                L->current->next = NULL;
            } // end if(!closeMill...
        } // end if(board[i]...
    } // end for(int i...
}

void Morris::neighbors(int p)
{
    for (int i = 0; i < 4; i++)
    {
        neigh[i] = -1;
    }
    switch (p)
    {
    case 0:
        neigh[0] = 1;
        neigh[1] = 3;
        neigh[2] = 8;
        break;
    case 1:
        neigh[0] = 0;
        neigh[1] = 2;
        neigh[2] = 4;
        break;
    case 2:
        neigh[0] = 1;
        neigh[1] = 5;
        neigh[2] = 13;
        break;
    case 3:
        neigh[0] = 0;
        neigh[1] = 4;
        neigh[2] = 6;
        neigh[3] = 9;
        break;
    case 4:
        neigh[0] = 1;
        neigh[1] = 3;
        neigh[2] = 5;
        break;
    case 5:
        neigh[0] = 2;
        neigh[1] = 4;
        neigh[2] = 7;
        neigh[3] = 12;
        break;
    case 6:
        neigh[0] = 3;
        neigh[1] = 7;
        neigh[2] = 10;
        break;
    case 7:
        neigh[0] = 5;
        neigh[1] = 6;
        neigh[2] = 11;
        break;
    case 8:
        neigh[0] = 0;
        neigh[1] = 9;
        neigh[2] = 20;
        break;
    case 9:
        neigh[0] = 3;
        neigh[1] = 8;
        neigh[2] = 10;
        neigh[3] = 17;
        break;
    case 10:
        neigh[0] = 6;
        neigh[1] = 9;
        neigh[2] = 14;
        break;
    case 11:
        neigh[0] = 7;
        neigh[1] = 12;
        neigh[2] = 16;
        break;
    case 12:
        neigh[0] = 5;
        neigh[1] = 11;
        neigh[2] = 13;
        neigh[3] = 19;
        break;
    case 13:
        neigh[0] = 2;
        neigh[1] = 12;
        neigh[2] = 22;
        break;
    case 14:
        neigh[0] = 10;
        neigh[1] = 15;
        neigh[2] = 17;
        break;
    case 15:
        neigh[0] = 14;
        neigh[1] = 16;
        neigh[2] = 18;
        break;
    case 16:
        neigh[0] = 11;
        neigh[1] = 15;
        neigh[2] = 19;
        break;
    case 17:
        neigh[0] = 9;
        neigh[1] = 14;
        neigh[2] = 18;
        neigh[3] = 20;
        break;
    case 18:
        neigh[0] = 15;
        neigh[1] = 17;
        neigh[2] = 19;
        neigh[3] = 21;
        break;
    case 19:
        neigh[0] = 12;
        neigh[1] = 16;
        neigh[2] = 18;
        neigh[3] = 22;
        break;
    case 20:
        neigh[0] = 8;
        neigh[1] = 17;
        neigh[2] = 21;
        break;
    case 21:
        neigh[0] = 18;
        neigh[1] = 20;
        neigh[2] = 22;
        break;
    case 22:
        neigh[0] = 13;
        neigh[1] = 19;
        neigh[2] = 21;
        break;
    default: break;
    } // end switch
}

bool Morris::closeMill(int p, char b[])
{
    char c = b[p];
    switch (p)
    {
    case 0:
        if ((b[1] == c && b[2] == c) || (b[3] == c && b[6] == c) || (b[8] == c && b[20] == c))
            return true;
        else return false;
    case 1:
        if (b[0] == c && b[2] == c)
            return true;
        else return false;
    case 2:
        if ((b[0] == c && b[1] == c) || (b[5] == c && b[7] == c) || (b[13] == c && b[22] == c))
            return true;
        else return false;
    case 3:
        if ((b[0] == c && b[6] == c) || (b[4] == c && b[5] == c) || (b[9] == c && b[17] == c))
            return true;
        else return false;
    case 4:
        if (b[3] == c && b[5] == c)
            return true;
        else return false;
    case 5:
        if ((b[3] == c && b[4] == c) || (b[2] == c && b[7] == c) || (b[12] == c && b[19] == c))
            return true;
        else return false;
    case 6:
        if ((b[0] == c && b[3] == c) || (b[10] == c && b[14] == c))
            return true;
        else return false;
    case 7:
        if ((b[2] == c && b[5] == c) || (b[11] == c && b[16] == c))
            return true;
        else return false;
    case 8:
        if ((b[0] == c && b[20] == c) || (b[9] == c && b[10] == c))
            return true;
        else return false;
    case 9:
        if ((b[8] == c && b[10] == c) || (b[3] == c && b[17] == c))
            return true;
        else return false;
    case 10:
        if ((b[6] == c && b[14] == c) || (b[8] == c && b[9] == c))
            return true;
        else return false;
    case 11:
        if ((b[7] == c && b[16] == c) || (b[12] == c && b[13] == c))
            return true;
        else return false;
    case 12:
        if ((b[5] == c && b[19] == c) || (b[11] == c && b[13] == c))
            return true;
        else return false;
    case 13:
        if ((b[2] == c && b[22] == c) || (b[11] == c && b[12] == c))
            return true;
        else return false;
    case 14:
        if ((b[6] == c && b[10] == c) || (b[15] == c && b[16] == c) || (b[17] == c && b[20] == c))
            return true;
        else return false;
    case 15:
        if ((b[14] == c && b[16] == c) || (b[18] == c && b[21] == c))
            return true;
        else return false;
    case 16:
        if ((b[7] == c && b[11] == c) || (b[14] == c && b[15] == c) || (b[19] == c && b[22] == c))
            return true;
        else return false;
    case 17:
        if ((b[3] == c && b[9] == c) || (b[14] == c && b[20] == c) || (b[18] == c && b[19] == c))
            return true;
        else return false;
    case 18:
        if ((b[15] == c && b[21] == c) || (b[17] == c && b[19] == c))
            return true;
        else return false;
    case 19:
        if ((b[5] == c && b[12] == c) || (b[16] == c && b[22] == c) || (b[17] == c && b[18] == c))
            return true;
        else return false;
    case 20:
        if ((b[0] == c && b[8] == c) || (b[14] == c && b[17] == c) || (b[21] == c && b[22] == c))
            return true;
        else return false;
    case 21:
        if ((b[15] == c && b[18] == c) || (b[20] == c && b[22] == c))
            return true;
        else return false;
    case 22:
        if ((b[2] == c && b[13] == c) || (b[16] == c && b[19] == c) || (b[20] == c && b[21] == c))
            return true;
        else return false;
    default: return false;
    } // end switch
}

int Morris::staticEstOpen(char board[])
{
    leaves++;
    numWhitePieces = 0;
    numBlackPieces = 0;
    for (int i = 0; i < 23; i++)
    {
        if (board[i] == 'W') numWhitePieces++;
        else if (board[i] == 'B') numBlackPieces++;
    }
    return (numWhitePieces - numBlackPieces);
}

int Morris::staticEstMidEnd(char board[])
{
    leaves++;
    numWhitePieces = 0;
    numBlackPieces = 0;
    for (int i = 0; i < 23; i++)
    {
        if (board[i] == 'W') numWhitePieces++;
        else if (board[i] == 'B') numBlackPieces++;
    }
    for (int i = 0; i < 23; i++)
    {
        if (board[i] == 'W') board[i] = 'B';
        else if (board[i] == 'B') board[i] = 'W';
    }
    Board *L = new Board();
    L = GenerateMovesMidgameEndgame(board);
    numBlackMoves = 0;
    L->current = L->head;
    while (L->current != NULL)
    {
        L->current = L->current->next;
        numBlackMoves++;
    }
    if (numBlackPieces <= 2) return 10000;
    else if (numWhitePieces <= 2) return (-10000);
    else if (numBlackMoves == 0) return 10000;
    else return (1000 * (numWhitePieces - numBlackPieces) - numBlackMoves);
}

int Morris::staticEstImproved(char board[])
{
    leaves++;
    int value = 0;
    char bCopy[23];
    for (int k = 0; k < 23; k++)
    {
        bCopy[k] = board[k];
    }
    for (int i = 0; i < 23; i++)
    {
        if (board[i] == 'W')
        {
            value++;
            if (closeMill(i, board)) value += 12;
            bCopy[i] = 'B';
            if (closeMill(i, bCopy)) value += 8;
        }
        else if (board[i] == 'B')
        {
            value--;
            if (closeMill(i, board)) value -= 6;
            bCopy[i] = 'W';
            if (closeMill(i, bCopy)) value -= 4;
        }
    }
    return value;
}

Morris::Board* Morris::addNode(Board *node, char position[])
{
    node = new Board();
    for (int i = 0; i < 23; i++)
    {
        node->boardB[i] = position[i];
    }
    node->child = NULL;
    node->nextSibling = NULL;
    node->currentSibling = node;
    return node;
}

Morris::Board* Morris::makeTree(char position[], int height, Board *node, int counter)
{
    isWhite = (counter % 2 != 0);
    Board *buffer = new Board();
    if (node == NULL)
    {
        buffer = addNode(node, position);
        node = buffer;
    }
    if (counter < height)
    {
        buffer = new Board();
        buffer = GenerateMovesOpening(position);
        //buffer = GenerateMovesMidgameEndgame(position);
        buffer->current = buffer->head;
        while (buffer->current != NULL)
        {
            if (node->child == NULL)
            {
                node->child = makeTree(buffer->current->boardB, height, node->child, ++counter);
            }
            else
            {
                Board *sibling = new Board();
                sibling = makeTree(buffer->current->boardB, height, node->child->currentSibling->nextSibling, counter);
                node->child->currentSibling->nextSibling = sibling;
                node->child->currentSibling = node->child->currentSibling->nextSibling;
            }
            buffer->current = buffer->current->next;
        }
    }
    return node;
}

int Morris::max(int a, int b)
{
    if (a > b) return a;
    else return b;
}

int Morris::min(int a, int b)
{
    if (a < b) return a;
    else return b;
}

int Morris::MaxMin(Board *position)
{
    if (position->child == NULL)
    {
        //return staticEstOpen(position->boardB);
        //return staticEstMidEnd(position->boardB);
        return staticEstImproved(position->boardB);
    }
    else
    {
        int v = INT_MIN;
        position->currentSibling = position->currentSibling->child;
        while (position->currentSibling->nextSibling != NULL)
        {
            v = max(v, MinMax(position->currentSibling));
            position->currentSibling = position->currentSibling->nextSibling;
        }
        return v;
    }
}

int Morris::MinMax(Board *position)
{
    if (position->child == NULL)
    {
        //return staticEstOpen(position->boardB);
        //return staticEstMidEnd(position->boardB);
        return staticEstImproved(position->boardB);
    }
    else
    {
        int v = INT_MAX;
        position->currentSibling = position->currentSibling->child;
        while (position->currentSibling->nextSibling != NULL)
        {
            v = min(v, MaxMin(position->currentSibling));
            position->currentSibling = position->currentSibling->nextSibling;
        }
        return v;
    }
}

int Morris::ABMaxMin(Board *position, int least, int greatest)
{
    if (position->child == NULL)
    {
        //return staticEstOpen(position->boardB);
        //return staticEstMidEnd(position->boardB);
        return staticEstImproved(position->boardB);
    }
    else
    {
        int v = INT_MIN;
        position->currentSibling = position->currentSibling->child;
        while (position->currentSibling->nextSibling != NULL)
        {
            v = max(v, ABMinMax(position->currentSibling, least, greatest));
            if (v >= greatest) return v;
            else least = max(v, least);
            position->currentSibling = position->currentSibling->nextSibling;
        }
        return v;
    }
}

int Morris::ABMinMax(Board *position, int least, int greatest)
{
    if (position->child == NULL)
    {
        //return staticEstOpen(position->boardB);
        //return staticEstMidEnd(position->boardB);
        return staticEstImproved(position->boardB);
    }
    else
    {
        int v = INT_MAX;
        position->currentSibling = position->currentSibling->child;
        while (position->currentSibling->nextSibling != NULL)
        {
            v = min(v, ABMaxMin(position->currentSibling, least, greatest));
            if (v <= least) return v;
            else greatest = min(v, greatest);
            position->currentSibling = position->currentSibling->nextSibling;
        }
        return v;
    }
}

void Morris::printTree(Board *node, int height, int combo)
{
    if (node != NULL)
    {
        //cout << "Height: " << height << endl;
        //cout << "Combo: " << combo << endl;
        for (int i = 0; i < 23; i++)
        {
            cout << node->boardB[i];
        }
        cout << endl;
        printTree(node->nextSibling, height, ++combo);
        printTree(node->child, ++height, 0);
    }
}

void Morris::printBoard(char b[])
{
    cout << b[20] << "------" << b[21] << "------" << b[22] << endl;
    cout << "|\\     |     /|" << endl;
    cout << "| " << b[17] << "----" << b[18] << "----" << b[19] << " |" << endl;
    cout << "| |\\   |   /| |" << endl;
    cout << "| | " << b[14] << "--" << b[15] << "--" << b[16] << " | |" << endl;
    cout << "| | |     | | |" << endl;
    cout << b[8] << "-" << b[9] << "-" << b[10] << "     " << b[11] << "-" << b[12] << "-" << b[13] << endl;
    cout << "| | |     | | |" << endl;
    cout << "| | " << b[6] << "-----" << b[7] << " | |" << endl;
    cout << "| |/       \\| |" << endl;
    cout << "| " << b[3] << "----" << b[4] << "----" << b[5] << " |" << endl;
    cout << "|/     |     \\|" << endl;
    cout << b[0] << "------" << b[1] << "------" << b[2] << endl;
}

void Morris::testList(char board[], int levels, string output)
{
    isWhite = true;
    root = NULL;

    cout << "Original:\n";
    for (int x = 0; x < 23; x++)
    {
        cout << board[x];
    }
    cout << endl << endl;

    root = makeTree(board, levels, root, 1);
    //printTree(root, 0, 1);

    root->currentSibling = root->child;
    root->currentSibling = root->currentSibling->nextSibling;
    char bestBoard[23];
    int bestEstimate = INT_MIN;
    int temp;
    int totalLeaves = 0;
    while (root->currentSibling != NULL)
    {
        cout << "possible move:\n";
        for (int x = 0; x < 23; x++)
        {
            cout << root->currentSibling->boardB[x];
        }
        cout << endl;
        leaves = 0;
        //temp = ABMaxMin(root->currentSibling, INT_MIN, INT_MAX);
        temp = MaxMin(root->currentSibling);
        totalLeaves += leaves;
        if (temp > bestEstimate)
        {
            bestEstimate = temp;
            for (int i = 0; i < 23; i++)
            {
                bestBoard[i] = root->currentSibling->boardB[i];
            }
        }
        cout << "Minimax Estimation: " << temp;
        //cout << "Alpha-Beta Minimax Estimation: " << temp;
        //cout << MaxMin(root->currentSibling);
        //cout << ABMinMax(root->currentSibling, INT_MIN, INT_MAX);
        //cout << MinMax(root->currentSibling);
        cout << "\nNumber of positions evaluated by static estimation: " << leaves << endl;
        root->currentSibling = root->currentSibling->nextSibling;
        cout << endl << endl;
    }
    cout << "\n**********************************************************";
    cout << "\n\nNext Move: ";
    for (int i = 0; i < 23; i++)
    {
        cout << bestBoard[i];
    }
    cout << "\nNumber of positions evaluated by static estimation: " << totalLeaves << endl;
    //cout << "Alpha-Beta Minimax Estimation: " << bestEstimate;
    cout << "Minimax Estimation: " << bestEstimate;
    cout << endl << endl;
    cout << "Before:\n";
    printBoard(board);
    cout << "\nAfter:\n";
    printBoard(bestBoard);
    ofstream outFile;
    outFile.open(output);
    for (int i = 0; i < 23; i++)
    {
        outFile << bestBoard[i];
    }
    outFile.close();
}

int main()
{
    char c;
    char board[23];
    string inputFile, outputFile;
    int depth;
    cout << "inputFile outputFile depth --> ";
    cin >> inputFile >> outputFile >> depth;
    ifstream input(inputFile);
    for (int i = 0; i < 23; i++) // initialize the board to the contents of board1.txt
    {
        input.get(c);
        board[i] = c;
    }
    Morris *M = new Morris();
    M->testList(board, depth, outputFile);
    //cout << "\n\nNumber of possible moves: a little less than " << moves << endl;
    cin >> c;
    return 0;
}