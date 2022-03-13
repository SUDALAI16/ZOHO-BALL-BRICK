#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
struct Base {
    int left;
    int right;
    int ball_pos;
}base;
void initGame(int n,char a[][n][10]) {
    int i,j;
    for(i=1;i<n-1;i++)
    {
        for(j=1;j<n-1;j++){
            a[i][j][0] = '-';
            a[i][j][1] = '\0';
        }
    }
    for(i=0;i<n;i++){
        a[i][n-1][0]=a[i][0][0]=a[0][i][0]='W';
        a[i][n-1][1]=a[i][0][1]=a[0][i][1]='\0';
    }
    for(i=1;i<n-1;i++){
        a[n-1][i][0] = 'G';
        a[n-1][i][1] = '\0';
    }
    a[n-1][n/2][0] = 'o';
    a[n-1][n/2][1] = '\0';
}

void displayGame(int n,char a[][n][10],int ball_count) {
    int i,j;

    printf("\n-----------------------------------------\n");
    for(i=0;i<n;i++) {
        for(j=0;j<n;j++) {
            if(a[i][j][0]=='-')
                printf("  ");
            else
                printf("%s ",a[i][j]);
        }
        printf("\n");
    }
    printf("\nBall count is : %d",ball_count);
    printf("\n-----------------------------------------\n");
}

char findType(char s[])
{
    if(isdigit(s[0]))
        return 'n';
    if(strcmp(s,"DE") == 0)
        return 'e';
    if(strcmp(s,"DS") == 0)
        return 's';
    if(strcmp(s,"B") == 0)
        return 'p';
    return '-';
}

void setBase(int n, char a[][n][10])
{
    for(int i=1;i<n-1;i++)
    {
        if(i==base.ball_pos)
            strcpy(a[n-1][i],"o");
        else if(i<base.left || i>base.right)
            strcpy(a[n-1][i],"G");
        else
            strcpy(a[n-1][i],"_");
    }
}
void hitBrick(int n, char a[][n][10],int i,int j,char type, int *brick_count){

    static int extend = 0;
    int strength;
    switch(type)
    {
        case 'n':   // numbered brick
            sscanf(a[i][j],"%d",&strength);
            strength--;
            if(strength)
                sprintf(a[i][j],"%d",strength);
            else{
                sprintf(a[i][j],"%s","-");
                *brick_count-=1;
            }
            break;
        case 'e':   // brick destroys entire row
            for(int k=1;k<n-1;k++)
            {
                if(findType(a[i][k])!='-')
                {
                    sprintf(a[i][k],"%s","-");
                    *brick_count-=1;
                }
            }
            break;
        case 's': // brick destroys surrounding bricks
            sprintf(a[i][j],"%s","-");
            *brick_count-=1;
            if(i-1!=0)
            {
                for(int k=j-1;k<=j+1;k++)
                {
                    if(k && k<n-1 && findType(a[i-1][k])!='-')
                    {
                        sprintf(a[i-1][k],"%s","-");
                        *brick_count-=1;
                    }
                }
            }
            if(i+1!=n-1)
            {
                for(int k=j-1;k<=j+1;k++)
                {
                    if(k && k<n-1 && findType(a[i+1][k])!='-')
                    {
                        sprintf(a[i+1][k],"%s","-");
                        *brick_count-=1;
                    }
                }
            }
            if(j-1!=0)
            {
                sprintf(a[i][j-1],"%s","-");
                *brick_count-=1;
            }
            if(j+1!=n-1)
            {
                sprintf(a[i][j+1],"%s","-");
                *brick_count-=1;
            }
            break;
        case 'p': // Powered brick
            sprintf(a[i][j],"%s","-");
            *brick_count-=1;
            if(!extend && base.right<n-2)
                base.right += 1;
            else if(extend && base.left>1)
                base.left -= 1;
            extend = !extend;
            break;
    }
    base.ball_pos = j;
    setBase(n,a);
}

int main()
{
    int n,i,j;
    int brick_count = 0;

    printf("Enter size of the NxN matrix : ");
    scanf("%d",&n);

    char a[n][n][10];

    initGame(n,a);

    // Getting brick inputs
    char choice;
    do {

        int x,y; // brick position
        char strength[10]; // brick strength
//        int strength; // brick strength

        printf("\nEnter the brick's position and the brick type : ");
        scanf("%d %d %s",&x,&y,strength);

        if(x>=n || y>=n || x<0 || y<0)
            printf("\nInvalid brick position !!!");
        else if(x==0 || y==0 || x==n-1 || y==n-1)
            printf("\nYou cant place the brick in the Wall or Ground !!!");
        else if(a[x][y][0]=='-') {
            sprintf(a[x][y],"%s",strength);
            brick_count++;
        }
        else
            printf("\nAlready a brick is placed here !!!");

        printf("\nDo you want to continue (Y or N) ? ");
        scanf("\n%c",&choice);

        choice = tolower(choice);
        while(choice!='y' && choice!='n') {
            printf("\nEnter valid choice (Y or N) ? ");
            scanf("\n%c",&choice);
            choice = tolower(choice);
        }

    }while(choice=='y');

    int ball_count;
    printf("\nEnter ball Count : ");
    scanf("%d",&ball_count);

    // #Question 1

    displayGame(n,a,ball_count);

    // #Question 2

    int btm = n-1;

    base.left = base.right = base.ball_pos = n/2;
    while(ball_count>0)
    {
        char direction[2];
        int strength;
        printf("\nEnter the direction in which the ball need to traverse : ");
        scanf("%s",direction);

        int flag=0,isLost=0;
        char type;

        switch(direction[0])
        {
            case 'S': // Straight (ST)
                for(i=btm-1;i>0;i--)
                {
                    type = findType(a[i][base.ball_pos]);
                    //printf("\n%c\n",type);
                    if(type!='-')
                        break;
                }
                if(i)
                    hitBrick(n,a,i,base.ball_pos,type,&brick_count);
                break;
            case 'L': // Left Diagonal (LD)
                for(i=btm-1,j=base.ball_pos-1;j>0;i--,j--)
                {
                    type = findType(a[i][j]);
                    if(type!='-')
                        break;
                }
                if(j)
                {
                    // printf("\n------------%d %d\n",i,j);
                    hitBrick(n,a,i,j,type,&brick_count);
                    if(j<base.left || j>base.right){
                        ball_count--;
                        isLost=1;
                    }
                }
                else
                {
                    for(j=1;j<n-1;j++) // Go right when hit left wall
                    {
                        type = findType(a[i][j]);
                        if(type!='-')
                            break;
                    }
                    if(j!=n-1)
                    {
                        hitBrick(n,a,i,j,type,&brick_count);
                        if(j<base.left || j>base.right){
                            ball_count--;
                            isLost=1;
                        }
                    }
                    else
                        ball_count--;
                }
                break;
            case 'R':   // Right Diagonal (RD)
                for(i=btm-1,j=base.ball_pos+1;j<n-1;i--,j++)
                {
                    type = findType(a[i][j]);
                    if(type!='-')
                        break;
                }
                if(j!=n-1)
                {
                    hitBrick(n,a,i,j,type,&brick_count);
                    if(j<base.left || j>base.right){
                        ball_count--;
                        isLost=1;
                    }
                }
                else
                {
                    for(j=n-2;j>0;j--) // Go Left when hit right wall
                    {
                        type = findType(a[i][j]);
                        if(type!='-')
                            break;
                    }
                    if(j)
                    {
                        hitBrick(n,a,i,j,type,&brick_count);
                        if(j<base.left || j>base.right){
                            ball_count--;
                            isLost=1;
                        }
                    }
                    else
                        ball_count--;
                }
                break;
            default:
                flag=1;
        }
        if(flag)
        {
            printf("\nInvalid direction !!!");
            continue;
        }

        displayGame(n,a,ball_count);
        if(isLost){
            base.ball_pos = n/2;
            setBase(n,a);
        }
        if(!brick_count)
            break;
    }

    if(!brick_count)
        printf("\n<---- YOU WON THE GAME ---->");
    else
        printf("\n<---- GAME OVER ---->");

    return 0;
}
