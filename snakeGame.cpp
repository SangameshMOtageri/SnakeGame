#include<stdio.h>
#include<iostream>
#include<string>
#include<time.h>
#include<thread>
#include<vector>

#define WINDOW_LENGTH 20
#define WINDOW_WIDTH 30

using namespace std;

struct snake_node{
    unsigned int x;
    unsigned int y;
    struct snake_node *next;
};

char board[WINDOW_LENGTH][WINDOW_WIDTH] = {' '};
vector<snake_node*> nodes;

char dir = 'd';
unsigned int l=6, f_posx=1, f_posy=1;
char snake[] = {'*','-'};
bool updateFood = true;

struct snake_node* new_node(unsigned int y, unsigned int x){
    struct snake_node *temp = new snake_node;
    temp->x = x;
    temp->y = y;
    temp->next = NULL;
    return temp;
}

void setBoard(){
    unsigned int i,j;
    for(i = 0; i < WINDOW_WIDTH; i++){
    board[0][i] = '#';
    board[WINDOW_LENGTH - 1][i] = '#';
    }
    for(i = 1; i < WINDOW_LENGTH - 1; i++){
        board[i][0] = '#';
        for(j = 1; j < WINDOW_WIDTH - 1; j++){
            board[i][j] = ' ';
        }
        board[i][WINDOW_WIDTH - 1] = '#';
    }
}

void updateBoard(vector<snake_node*> head, unsigned int length){

    if(updateFood == true){
        unsigned int food_location = rand();
        food_location = food_location%((WINDOW_LENGTH - 2)*(WINDOW_WIDTH - 2));
    /*Clear previous food position*/
        board[f_posy][f_posx] = ' ';
    /*Update new food position*/
        f_posx = 1 + food_location%(WINDOW_WIDTH - 2);
        f_posy = 1 + food_location/(WINDOW_WIDTH - 2);
        updateFood = false;
    }

    board[f_posy][f_posx] = '@';
    for(unsigned int i=0;i<length;i++){
        board[head[i]->y][head[i]->x] = snake[i%2];
    }
}

void printBoard(){
    cout<<"\n\n\t\tWelcome to the snake game!!\n\n";
    for(unsigned int i=0;i<WINDOW_LENGTH;i++){
        cout<<"\n\t\t";
        for(unsigned int j=0;j<WINDOW_WIDTH;j++)
            cout<<board[i][j];
    }
}

void getDirection(char& dir){
    while(true){
        cin>>dir;
    }
}

void checkBoundary(){
    while(true){
        if(nodes[0]->x <0 || nodes[0]->x >=WINDOW_WIDTH || 
            nodes[0]->y <0 || nodes[0]->y >=WINDOW_LENGTH)
            {
                cout<<"\nWe have a boundary violation\n";
                abort();
            }
        for(unsigned int i=3;i<l;i++){
            if(nodes[i]->x == nodes[0]->x && nodes[i]->y == nodes[0]->y){
                cout<<"\nSnake bit itself, you lose\n";
                abort();
            }
            if(nodes[i]->x == f_posx && nodes[i]->y == f_posy)
                updateFood = true;
        }
        if(((nodes[0]->x == f_posx && nodes[0]->y == f_posy) ||
            (nodes[1]->x == f_posx && nodes[1]->y == f_posy)) &&
            (updateFood == false)){
                updateFood = true;
                l++;
                nodes.push_back(new_node(nodes[1]->y, nodes[1]->x));
            }
    }
}

int main(){
    unsigned int i, j;
/*Default start size and start location*/

    for(i=10;i>=5;i--)
        nodes.push_back(new_node(1, i));
    for(i=l-1;i>0;i--){
        nodes[i]->next = nodes[i-1];
    }
    setBoard();
    std::thread t1(getDirection, std::ref(dir));
    std::thread t2(checkBoundary);
    while(dir != 'x'){
        updateBoard(nodes, l);
        printBoard();
/*Set last part of tail to null*/
        board[nodes[l-1]->y][nodes[l-1]->x] = ' ';
        for(i=l-1;i>0;i--){
            nodes[i]->x = nodes[i-1]->x;
            nodes[i]->y = nodes[i-1]->y;
        }
        switch(dir){
            case 'w':
                nodes[0]->y--;
                break;
            case 'a':
                nodes[0]->x--;
                break;
            case 's':
                nodes[0]->y++;
                break;
            case 'd':
                nodes[0]->x++;
                break;
        }
        _sleep(5);
        system("CLS");
    }
    t1.join();
    t2.join();
}