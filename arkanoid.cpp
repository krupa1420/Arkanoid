#include <iostream>
#include<graphics.h>
#include <process.h>
# include<stdlib.h>
# include<stdio.h>
#include<conio.h>
#include<dos.h>
// To display the bricks (in square box) and paddles in rectangle form and bulbs rounded form
void bricks();
void drawbrick(int,int);
void erasebrick(int,int);
void mainscreen();
void youwin();
void youloss();
char playerlevel();
int maxx, maxy, midx, midy;
int bri[5][18];
int main()
{
    static int ballx, bally, paddlex, paddley, dx = 1, dy = -1, oldx, oldy,score=0,ch=0,area;
    int flag=0,limit = 100, currentlayer = 4,chance=3 ;
    int layer[5]={25,50,75,100,125};
    char cplayerlevel;
    void *p,*b;
    char gscore[20],gchance[10];
    int speed=1; //fast by default
     initwindow(900,500,"Play the Game");
     mainscreen();
     cplayerlevel = playerlevel();
    switch (cplayerlevel) {
    case 'M':
        speed = 15;
        break;

    case 'S':
        speed = 25;
        break;
    }
     initwindow(900,400,"Play the Game");
     /* get the maximum x and y screen coordinates */
    maxx = getmaxx();
    maxy = getmaxy();

    /* calculate center of screen */
    midx = maxx / 2;
    midy = maxy / 2;

    /* draw the four layer of bricks, the paddle and the ball*/
    setfillstyle(SOLID_FILL,BLACK);
	bar(0,0,maxx , maxy -15);

    bricks();
    setfillstyle(SOLID_FILL,GREEN);
	bar(midx- 40,maxy- 7 - 60,midx + 40,maxy- 60);

    setfillstyle(SOLID_FILL,YELLOW);
    //circle(midx,maxy- 15 - 12, 15);
    fillellipse(midx,maxy- 22 - 60,12,15 ) ;

    setlinestyle(DOTTED_LINE,0,2);
    line(0,maxy-55,900,maxy-55);
    /* store current position of the paddle and ball */
    paddlex = midx - 40;
    paddley = maxy - 7 - 60;
    ballx = midx - 12;
    bally =maxy - 22 - 60 - 15;

    //to show score and balls
    sprintf(gscore,"score : %d",score);
    settextstyle(DEFAULT_FONT, HORIZ_DIR,3);
    outtextxy(30,maxy-40,gscore);
    sprintf(gchance,"life : %d",chance);
    settextstyle(DEFAULT_FONT, HORIZ_DIR,3);
    outtextxy(500,maxy-40,gchance);


    //imagesize and getimage
    area = imagesize(midx- 40,maxy- 7 - 60,midx + 40,maxy- 60);
    p=malloc(area);
    getimage(midx- 40,maxy- 7 - 60,midx + 40,maxy- 60, p);
    area = imagesize(midx - 12,maxy- 22 - 60 -15 ,midx + 12,maxy- 22 - 60 + 14);
    b=malloc(area);
    getimage(midx - 12,maxy- 22 - 60 -15 ,midx + 12,maxy- 22 - 60 + 14, b);


    while(1)
    {

         /* save the current x and y coordinates of the ball */
        flag = 0;
        oldx = ballx;
        /* update ballx and bally to move the ball in appropriate direction */
        oldy = bally;
        ballx = ballx + dx;
        /* as per the position of ball determine the layer of bricks to check */
        bally = bally + dy;

        /* if the ball hits the left boundary, deflect it to the right */
        if (ballx < 1) {
            ballx = 1;
            dx = -dx;
        }
        /* if the ball hits the right boundary, deflect it to the left */
        if (ballx > (maxx -25)) {
          ballx = maxx - 25;
            dx =-dx;
        }
        /* if the ball hits the top boundary, deflect it down */
        if (bally < 1) {
            bally = 1;
            dy = -dy;
        }
        //get the current layer and limit
       if (bally > 100) {
            limit = 125;
            currentlayer = 4;
        }
        else {
            if (bally > 75) {
                limit = 100;
                currentlayer = 3;
            }
            else {
                if (bally > 50) {
                    limit = 75;
                    currentlayer = 2;
                }
                else {
                    if (bally > 25) {
                        limit = 50;
                        currentlayer = 1;
                    }
                    else {
                        limit = 25;
                        currentlayer = 0;
                    }
                }
            }
        }

        /* if the ball is in the area occupied by the bricks */
        if (bally < limit) {
            /* if there is no brick present exactly at the top of the ball */
            if (bri[currentlayer][(ballx + 25) / 50] == 1) {
                /* determine if the boundary of the ball touches a brick */
                for (int i = 1; i <= 6; i++) {
                    /* check whether there is a brick to the right of the ball */
                    if (bri[currentlayer][(ballx + i + 25) / 50] == 0) {
                        /* if there is a brick */
                        ballx = ballx + i;
                        flag = 1;
                        break;
                    } /* check whether there is a brick to the left of the ball */
                    if (bri[currentlayer][(ballx - i + 25) / 50] == 0) {
                        ballx = ballx - i;
                        flag = 1;
                        break;
                    }
                } /* if the ball does not touch a brick at the top, left or right */
                if (!flag) {
                    /* check if the ball has moved above the current layer */
                    if (bally < layer[currentlayer - 1]) {
                        /* if so, change current layer appropriately */
                        currentlayer--;
                        limit = layer[currentlayer];
                    } /* put the image of the ball at the old coordinates */
                    /* introduce delay */
                    putimage(oldx, oldy, b, OR_PUT);
                    putimage(oldx, oldy, b, XOR_PUT);
                    putimage(ballx, bally, b, XOR_PUT);
                    delay(speed); /* carry on with moving the ball */
                    continue;
                }
            }
             erasebrick((ballx + 25) / 50, currentlayer);
             bri[currentlayer][(ballx + 25) / 50] = 1;
             bally = bally + 1; /* update the y coordinate */
            dy = -dy; /* change the direction of the ball */
            score += 5; /* increment score */
            sprintf(gscore,"score : %d",score);
            settextstyle(DEFAULT_FONT, HORIZ_DIR,3);
            outtextxy(30,maxy-40,gscore);
        }
         /* if the ball has reached the bottom */
        if(bally > 333 - 30)
        {
             /* if the paddle has missed the ball */
            if (ballx > paddlex + 40 || ballx < paddlex) {
                    /* update ballx and bally to move the ball in appropriate direction */
                    oldx = ballx;
                    oldy = bally;
                    ballx = ballx + dx;
                    bally = bally + dy;
                    chance--; /* decrement the number of chances */
                    score -= 20; /* decrement 20 points for each ball lost */
                    sprintf(gscore,"score : %d",score);
                    settextstyle(DEFAULT_FONT, HORIZ_DIR,3);
                    outtextxy(30,maxy-40,gscore);
                    sprintf(gchance,"life : %d",chance);
                    settextstyle(DEFAULT_FONT, HORIZ_DIR,3);
                    outtextxy(500,maxy-40,gchance);
                    if(chance == 0)
                    youloss();
                    getch();
            }
            /* if ball is collected on paddle */
            Beep(2211,100);
            bally =  333- 30 ; /* restore the y coordinate of ball */
            dy = -dy; /* deflect the ball upwards */
        }
        putimage(oldx, oldy, b, OR_PUT); /* erase the image of the ball at the old coordinates */
        putimage(oldx, oldy, b, XOR_PUT); /* put the image of the ball at the upadted coordinates */
        putimage(ballx, bally, b, XOR_PUT);
        delay(speed);
        /* if all the bricks have been destroyed */
        if (score == 450 - ((3 - chance) * 20))
        {
             youwin();
             getch();
        }
        delay(speed);
        if(kbhit())
        {
            ch = getch();
            putimage(paddlex,maxy- 7 - 60, p,OR_PUT);
            putimage(paddlex,maxy- 7 - 60, p,XOR_PUT);
            if(ch==77)    //right arrow key
            paddlex= paddlex+30;
            if(paddlex>=830)
            paddlex=820;
            if(ch==75)    //left arrow key
            paddlex=paddlex-30;
            if(paddlex<=0)
            paddlex=2;
            putimage(paddlex,maxy- 7 - 60, p,XOR_PUT);
        }


    }
    closegraph();
   getch();
}

void mainscreen()
{
	int maxx, maxy, in, area;

	// get maximum x, y coordinates of the screen
	maxx = getmaxx();
	maxy = getmaxy();

	// setbkcolor sets the current background color using the palette
	setbkcolor(YELLOW);

	// Draws a rectangle (graphics mode)
	rectangle(0, 0, maxx, maxy);

     setcolor(1);
      // settextstyle(font, orientation, size)
    settextstyle(3,0,3);
	// displaying the output text on main screen
	outtextxy(270, 40, " WELCOME TO ARKANOID ");
	setcolor(LIGHTCYAN);
    circle(40,170,5);
    circle(40,190,5);
    circle(40,210,5);
    circle(40,230,5);
    circle(40,250,5);
    setcolor(LIGHTMAGENTA);
	setbkcolor(BLACK);
    settextstyle(DEFAULT_FONT, HORIZ_DIR,4);
	outtextxy(20, 100, " Instructions ");
	setcolor(LIGHTGREEN);
	 settextstyle(DEFAULT_FONT, HORIZ_DIR, 0);
    outtextxy(50, 160, "Use left and right arrow keys to move paddle.");
    outtextxy(50, 180, "If you don't collect the ball on the paddle, you lose the ball.");
    outtextxy(50, 200, "On loosing a ball you loose 20 points.");
    outtextxy(50, 220, "On taking a brick you gain 5 points.");
    outtextxy(50, 240, "Press any key to continue... ");
	// Flushes the standard input device
	fflush(stdin);
	getch();
	closegraph();

}
/* This function draws bricks at the start of the game.There are four
layers of
the bricks  */
void bricks()
{
   int i, j, lx = 0, ly = 0;
    for (i = 0; i < 5; i++) /* 5 rows */ {
        for (j = 0; j < 18; j++) /* 18 columns */ {
            /* draw a brick at appropriate coordinates */
            drawbrick(lx, ly);
            lx = lx + 50;
        }
        lx = 0;
        ly = ly + 25;
    }
}
/* draws a brick at the proper position */
void drawbrick(int lx, int ly) {
    setfillstyle(SOLID_FILL,BLUE);
    //bar(lx, ly, lx + 49, ly + 24);
    bar(lx + 8, ly , lx + 49 - 8, ly + 24 - 8);

}

/* erases the specified brick */
void erasebrick(int b, int l) {
    /* b - brick number, l - layer */
    setfillstyle(SOLID_FILL,BLACK);
    bar((b * 50 )+ 8, l * 25,(b * 50 ) +  49 -8 ,(l * 25)  + 24 - 8);

}
/* you loss */
void youloss()
{
    initwindow(900,400,"Play the Game");
    setcolor(RED);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
    outtextxy(midx- 130, midy, "You loss !!!");
    fflush(stdin);
	getch();
	closegraph();

}
/* you loss */
void youwin()
{
    initwindow(900,400,"Play the Game");
    setcolor(YELLOW);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
    outtextxy(midx - 130, midy - 30, "Congratulations..");
    outtextxy(midx - 130, midy, "You win !!!");
    fflush(stdin);
	getch();
	closegraph();
}

char playerlevel()
{
    char ch;
    initwindow(900,400,"Play the Game");
    setcolor(LIGHTCYAN);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
   outtextxy(20, 125, "Select any of the following levels:");
   setcolor(LIGHTGRAY);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    outtextxy(20, 155, "Fatser ( F )");
    outtextxy(20, 185, "Medium ( M )");
    outtextxy(20, 215, "Slow ( S )"); /* get user's choice */
   fflush(stdin);
    ch = getch();
    return (ch);
}