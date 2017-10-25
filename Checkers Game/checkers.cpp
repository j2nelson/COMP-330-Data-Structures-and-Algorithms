#include "checkers.h"

Piece* pieces[8][8];
int sx,sy,ex,ey,aistartx,aistarty,aiendx,aiendy;
bool turn=true;
bool jumpaga=false;
bool aijumpaga=false;

//creates piece
Piece::Piece(bool is)
{
    isRed = is;
    isKing = false;
}

//draws piece
void Piece::draw(QPainter* paint, int x, int y)
{
  if(isRed == true)
  {
      if(isKing == false)
      {
          paint->setBrush(Qt::red);
      }
      else
      {
          paint->setBrush(Qt::green);
      }
  }
  else
  {
      if(isKing == false)
      {
          paint->setBrush(Qt::black);
      }
      else
      {
          paint->setBrush(Qt::blue);
      }
  }

  paint->drawEllipse(QPoint(x*80+80/2,y*80+80/2),80/2-4,80/2-4);
}

//checks if piece can move
bool Piece::canMove(int startx, int starty, int endx, int endy)
{
    int middlex, middley,movestartx,movestarty;
    //if second move
    if((jumpaga==true)||(aijumpaga==true))
    {
        movestartx = abs(startx-endx);
        movestarty = abs(starty-endy);
        if( movestartx != 2)
        {
            return false;
        }
        if( movestarty != 2)
        {
            return false;
        }
        return true;
    }
    else
    {
    //Only allow red pieces to move when turn is true and black pieces when turn is false.
    if(turn == true)
    {
        if(pieces[startx][starty]->isRed == false)
        {
            return false;
        }
    }
    if(turn == false)
    {
        if(pieces[startx][starty]->isRed == true)
        {
            return false;
        }
    }
    //Are startx, starty, endx, endy all between 0 and 7?  If not, return false
    if((startx < 0)||(startx > 7)||(starty < 0)||(starty > 7)||(endx <0)||(endx > 7)||(endy <0)||(endy > 7))
    {
        return false;
    }
    //A piece must land on an empty square.  Is piece[endx][endy] empty (NULL)?  If not return false.
    if(pieces[endx][endy] != NULL)
    {
        return false;
    }
    //If the piece isn't a King, it must move forward.  That is to say, if it's red, then endy must be < starty, and if it's black, endy>starty.  If this isn't the case, return false.
    if(pieces[startx][starty]->isKing == false)
    {
        if(pieces[startx][starty]->isRed == true)
        {
            if(endy > starty)
            {
                return false;
            }
        }
        else
        {
            if(endy < starty)
            {
                return false;
            }
        }
    }
    //Pieces can only move 1 space or 2.  Check that abs(startx-endx) and abs(starty-endy) are both either 1 or 2.  If not, return false.
    movestartx = abs(startx-endx);
    movestarty = abs(starty-endy);
    if(movestartx < 1 || movestartx > 2)
    {
        return false;
    }
    if(movestarty < 1 || movestarty >2)
    {
        return false;
    }
    //Pieces can only move diagonally.  Check that abs(startx-endx) is the same as abs(starty-endy).  If not, return false.
    if(abs(startx-endx) != abs(starty-endy))
    {
        return false;
    }
    //If the move is 2 steps, there must be a capture.  So if abs(startx-endx) is 2, then do the following:
    if(abs(startx-endx) == 2)
    {
        //Get the index of the square in the middle.  Try startx+(endx-startx)/2 to get the x.  Do the same thing to get the y.
        middlex = startx+(endx-startx)/2;
        middley = starty+(endy-starty)/2;
        //Then check:
        //Is that square empty?  You can't jump empty squares.  Return false.
        if(pieces[middlex][middley] == NULL)
        {
            return false;
        }
        //Is the piece in that square the same color as yourself?  You can't jump yourself.  Return false.
        if(pieces[startx][starty]->isRed == pieces[middlex][middley]->isRed)
        {
            return false;
        }
    }
    return true;
    }
}

//checks if jump can be made
bool Piece::jump(int start, int end)
{
    int cx,cy,ecx,ecy;

    //if a player can jump then the player must jump
        if(abs(start-end) == 2)
        {
            return true;
        }
        else
        {
            //Go through every possible piece (for x 0..8, for y 0..8) and check if there is a piece there of turn's color
            for(cx=0;cx<8;cx++)
            {
                for(cy=0;cy<8;cy++)
                {
                    if(pieces[cx][cy] != NULL)
                    {
                        if(pieces[cx][cy]->isRed == turn)
                        {
                            //Go through every possible destination (for endx 0..8, for endy 0..8) and check canMove
                            for(ecx=0;ecx<8;ecx++)
                            {
                                for(ecy=0;ecy<8;ecy++)
                                {
                                    if( pieces[cx][cy]->canMove(cx,cy,ecx,ecy) == true)
                                    {
                                        if(abs(cx-ecx)==2)
                                        {
                                            return false;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        return true;
}

//compute player
void Checkers::randomAI()
{
    int move, jx,jy, ejx,ejy;
    bool jumper;
    srand (time(NULL));
    int midx, midy;
    while(true)
    {
        //Call rand()%8 to pick a random number from 0 to 7.  Make four random numbers startx starty endx endy.
        aistartx= rand()%8;
        aistarty= rand()%8;
        aiendx= rand()%8;
        aiendy= rand()%8;
        //Check if there is a piece there (not NULL).  Call canMove and see if it's a valid move.
        if(pieces[aistartx][aistarty] != NULL)
        {
            if(pieces[aistartx][aistarty]->canMove(aistartx,aistarty,aiendx,aiendy)==true)
            {
                jumper = pieces[aistartx][aistarty]->jump(aistartx,aiendx);
                if( jumper == true)
                {
                    pieces[aiendx][aiendy]=pieces[aistartx][aistarty];
                    pieces[aistartx][aistarty]=NULL;
                    //check if it was a two square move
                    if(abs(aistartx-aiendx) == 2)
                    {
                        //If so, set the middle square to NULL to delete (capture) the opponent's piece.
                        midx = aistartx+(aiendx-aistartx)/2;
                        midy = aistarty+(aiendy-aistarty)/2;
                        pieces[midx][midy] = NULL;
                        //if a piece made a jump and can then make another jump, the player's turn isn't over.  The player can keep jumping a piece as long as it has jumps available
                        //aijumpaga = pieces[aiendx][aiendy]->jumpagain(aiendx,aiendy);
                        //if(aijumpaga == true)
                        //{
                            //turn=!turn;
                        //}
                    }
                    //check if a piece has reached the end (y = 0 for red, 7 for black).  If so, set its isKing to true.
                    if(pieces[aiendx][aiendy]->isRed == true)
                    {
                        if(aiendy == 0)
                        {
                            pieces[aiendx][aiendy]->isKing = true;
                        }
                    }
                    else
                    {
                        if(aiendy == 7)
                        {
                            pieces[aiendx][aiendy]->isKing = true;
                        }
                    }
                    turn=!turn;
                    repaint();
                    break;
                }
            }
        }
        //If it is, make the move, switch turns, update the screen, and break.  Otherwise, go back to step 1 and try again (think while(true) loop)
    }
    move=0;
    //Go through every possible piece (for x 0..8, for y 0..8) and check if there is a piece there of turn's color
    for(jx=0;jx<8;jx++)
    {
        for(jy=0;jy<8;jy++)
        {
            if(pieces[jx][jy] != NULL)
            {
                if(pieces[jx][jy]->isRed == turn)
                {
                    //Go through every possible destination (for endx 0..8, for endy 0..8) and check canMove
                    for(ejx=0;ejx<8;ejx++)
                    {
                        for(ejy=0;ejy<8;ejy++)
                        {
                            if( pieces[jx][jy]->canMove(jx,jy,ejx,ejy) == true)
                            {
                                move++;
                            }
                        }
                    }
                }
            }
        }
    }
    //If canMove is ever true, then we haven't finished the game yet
    // However,If it is never true, then !turn (the opposing player) has won.
    //Use setWindowTitle to declare victory, and don't accept any new moves.
    if(move == 0)
    {
        setWindowTitle("DEFEAT");
        for(jx=0;jx<8;jx++)
        {
            for(jy=0;jy<8;jy++)
            {
                pieces[jx][jy]= NULL;
            }
        }
        repaint();
    }
}

//check if piece can jump again
bool Piece::jumpagain(int endlocx, int endlocy)
{
    int ax,ay,bx,by,dx,dy,fx,fy;
    ax=endlocx-1;
    ay=endlocy-1;
    bx=endlocx+1;
    by=endlocy+1;
    dx=endlocx-2;
    dy=endlocy-2;
    fx=endlocx+2;
    fy=endlocy+2;
    if(pieces[endlocx][endlocy]->isKing == false)
    {
        if(pieces[endlocx][endlocy]->isRed == true)
        {
            if((endlocx > 0)&&(endlocy > 0))
            {
                if(pieces[ax][ay] != NULL)
                {
                    if(pieces[ax][ay]->isRed == false)
                    {
                        if((ax > 0)&&(ay>0))
                        {
                            if(pieces[dx][dy] == NULL)
                            {
                                return true;
                            }
                        }
                    }
                }
            }
            if((endlocx < 7)&&(endlocy > 0))
            {
                if(pieces[bx][ay] != NULL)
                {
                    if(pieces[bx][ay]->isRed == false)
                    {
                        if((bx < 7)&&(ay>0))
                        {
                            if(pieces[fx][dy] == NULL)
                            {
                                return true;
                            }
                        }
                    }
                }
            }
        }
        if(pieces[endlocx][endlocy]->isRed == false)
        {
            if((endlocx > 0)&&(endlocy <7))
            {
                if(pieces[ax][by] != NULL)
                {
                    if(pieces[ax][by]->isRed == true)
                    {
                        if((ax > 0)&&(by<7))
                        {
                            if(pieces[dx][fy] == NULL)
                            {
                                return true;
                            }
                        }
                    }
                }
            }
            if((endlocx < 7)&&(endlocy < 7))
            {
                if(pieces[bx][by] != NULL)
                {
                    if(pieces[bx][by]->isRed == true)
                    {
                        if((bx < 7)&&(by<7))
                        {
                            if(pieces[fx][fy] == NULL)
                            {
                                return true;
                            }
                        }
                    }
                }
            }
        }

    }
    else
    {
        if(pieces[endlocx][endlocy]->isRed == true)
        {
            if((endlocx > 0)&&(endlocy > 0))
            {
                if(pieces[ax][ay] != NULL)
                {
                    if(pieces[ax][ay]->isRed == false)
                    {
                        if((ax > 0)&&(ay>0))
                        {
                            if(pieces[dx][dy] == NULL)
                            {
                                return true;
                            }
                        }
                    }
                }
            }
            if((endlocx < 7)&&(endlocy > 0))
            {
                if(pieces[bx][ay] != NULL)
                {
                    if(pieces[bx][ay]->isRed == false)
                    {
                        if((bx <7)&&(ay>0))
                        {
                            if(pieces[fx][dy] == NULL)
                            {
                                return true;
                            }
                        }
                    }
                }
            }
            if((endlocx > 0)&&(endlocy < 7))
            {
                if(pieces[ax][by] != NULL)
                {
                    if(pieces[ax][by]->isRed == false)
                    {
                        if((ax > 0)&&(by<7))
                        {
                            if(pieces[dx][fy] == NULL)
                            {
                                return true;
                            }
                        }
                    }
                }
            }
            if((endlocx < 7)&&(endlocy < 7))
            {
                if(pieces[bx][by] != NULL)
                {
                    if(pieces[bx][by]->isRed == false)
                    {
                        if((bx <7)&&(by<7))
                        {
                            if(pieces[fx][fy] == NULL)
                            {
                                return true;
                            }
                        }
                    }
                }
            }
        }
        else
        {
            if(pieces[endlocx][endlocy]->isRed == false)
            {
                if((endlocx > 0)&&(endlocy > 0))
                {
                    if(pieces[ax][ay] != NULL)
                    {
                        if(pieces[ax][ay]->isRed == true)
                        {
                            if((ax > 0)&&(ay>0))
                            {
                                if(pieces[dx][dy] == NULL)
                                {
                                    return true;
                                }
                            }
                        }
                    }
                }
                if((endlocx < 7)&&(endlocy > 0))
                {
                    if(pieces[bx][ay] != NULL)
                    {
                        if(pieces[bx][ay]->isRed == true)
                        {
                            if((bx <7)&&(ay>0))
                            {
                                if(pieces[fx][dy] == NULL)
                                {
                                    return true;
                                }
                            }
                        }
                    }
                }
                if((endlocx > 0)&&(endlocy < 7))
                {
                    if(pieces[ax][by] != NULL)
                    {
                        if(pieces[ax][by]->isRed == true)
                        {
                            if((ax > 0)&&(by<7))
                            {
                                if(pieces[dx][fy] == NULL)
                                {
                                    return true;
                                }
                            }
                        }
                    }
                }
                if((endlocx < 7)&&(endlocy < 7))
                {
                    if(pieces[bx][by] != NULL)
                    {
                        if(pieces[bx][by]->isRed == true)
                        {
                            if((bx <7)&&(by<7))
                            {
                                if(pieces[fx][fy] == NULL)
                                {
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

//main window
Checkers::Checkers(QWidget *parent)
    : QMainWindow(parent)

{

    resize(W,H);
    int xx,yy;
    for(yy = 0; yy<8; yy++)
    {
        if((yy == 0)||(yy == 2))
        {
            for(xx=0; xx<8; xx++)
            {
                 if(xx%2 == 0)
                {
                        pieces[xx][yy] = new Piece(false);
                }
                 else
                {
                        pieces[xx][yy] = NULL;
                }
            }
        }
        if(yy == 1)
        {
             for(xx=0;xx<8;xx++)
            {
                if(xx%2 == 0)
                {
                        pieces[xx][yy] = NULL;
                }
                else
                {
                        pieces[xx][yy] = new Piece(false);
                }
            }
        }
        if ((yy == 3) || (yy==4))
        {
                for(xx=0; xx<8;xx++)
                {
                    pieces[xx][yy] = NULL;
                }
        }
        if ((yy == 5) || (yy== 7))
        {
            for(xx=0;xx<8;xx++)
            {
                if(xx%2 == 0)
                {
                    pieces[xx][yy] = NULL;
                }
                else
                {
                    pieces[xx][yy] = new Piece(true);
                }
            }
        }
        if (yy == 6)
        {
            for (xx=0;xx<8;xx++)
            {
                if(xx%2 == 0)
                {
                    pieces[xx][yy] = new Piece(true);
                }
                else
                {
                    pieces[xx][yy] = NULL;
                }
            }
        }
    }
    this->repaint();

}

//creates graphics
void Checkers::paintEvent(QPaintEvent *)
{
    QPainter paint(this);   //make an object to paint

    int x,y;
    for(x=0;x<8;x++)
    {
        for(y=0;y<8;y++)
        {
            if(x%2 == 0)
            {
                if(y%2 == 0)
                {
                    paint.fillRect(QRect(80*x,80*y,80,80),Qt::gray);
                }
                else
                {
                    paint.fillRect(QRect(80*x,80*y,80,80),Qt::white);
                }
            }
            else
            {
                if(y%2 == 0)
                {
                    paint.fillRect(QRect(80*x,80*y,80,80),Qt::white);
                }
                else
                {
                    paint.fillRect(QRect(80*x,80*y,80,80),Qt::gray);
                }
            }
        }
    }
    x=0;
    y=0;


        for (x=0; x<8; x++)
        {
            for (y=0; y<8; y++)
            {
                if(pieces[x][y] != NULL)
                {
                    pieces[x][y]->draw(&paint, x, y);
                }
            }
        }


}

//checks where mouse is pressed
void Checkers::mousePressEvent(QMouseEvent* event)
{
    sx = event->x()/80;
    sy = event->y()/80;
}


//checks where mouse is released
void Checkers::mouseReleaseEvent(QMouseEvent *e)
{
    int mx,my,ix,iy,eix,eiy, moves;
    ex=e->x()/80;
    ey=e->y()/80;
    bool jumps;
    if(pieces[sx][sy] != NULL)
    {

        if(pieces[sx][sy]->canMove(sx,sy,ex,ey)==true)
        {
            jumps = pieces[sx][sy]->jump(sx,ex);

            if(jumps == true)
            {

                pieces[ex][ey]=pieces[sx][sy];
                pieces[sx][sy]=NULL;
                //check if it was a two square move
                if(abs(sx-ex) == 2)
                {
                    //If so, set the middle square to NULL to delete (capture) the opponent's piece.
                    mx = sx+(ex-sx)/2;
                    my = sy+(ey-sy)/2;
                    pieces[mx][my] = NULL;
                    //if a piece made a jump and can then make another jump, the player's turn isn't over.  The player can keep jumping a piece as long as it has jumps available
                    jumpaga = pieces[ex][ey]->jumpagain(ex,ey);
                    if(jumpaga == true)
                    {
                        turn=!turn;
                    }
                }
                //check if a piece has reached the end (y = 0 for red, 7 for black).  If so, set its isKing to true.
                if(pieces[ex][ey]->isRed == true)
                {
                    if(ey == 0)
                    {
                        pieces[ex][ey]->isKing = true;
                    }
                }
                else
                {
                    if(ey == 7)
                    {
                        pieces[ex][ey]->isKing = true;
                    }
                }
                turn=!turn;
                repaint();
                moves=0;
                //Go through every possible piece (for x 0..8, for y 0..8) and check if there is a piece there of turn's color
                for(ix=0;ix<8;ix++)
                {
                    for(iy=0;iy<8;iy++)
                    {
                        if(pieces[ix][iy] != NULL)
                        {
                            if(pieces[ix][iy]->isRed == turn)
                            {
                                //Go through every possible destination (for endx 0..8, for endy 0..8) and check canMove
                                for(eix=0;eix<8;eix++)
                                {
                                    for(eiy=0;eiy<8;eiy++)
                                    {
                                        if( pieces[ix][iy]->canMove(ix,iy,eix,eiy) == true)
                                        {
                                            moves++;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                //If canMove is ever true, then we haven't finished the game yet
                // However,If it is never true, then !turn (the opposing player) has won.
                //Use setWindowTitle to declare victory, and don't accept any new moves.
                if(moves == 0)
                {
                    setWindowTitle("VICTORY");
                    for(ix=0;ix<8;ix++)
                    {
                        for(iy=0;iy<8;iy++)
                        {
                            pieces[ix][iy]= NULL;
                        }
                    }
                    repaint();
                }
            }
        }
    }





    //after you make a move, call a randomAI function to move the opponent.
    if((moves !=0)&&(jumps== true))
    {
        while(turn == false)
        {
            randomAI();
        }
    }


    qDebug()<<"pressed "<<sx<<","<<sy<<" released "<<ex<<","<<ey<<endl;
}




Checkers::~Checkers()
{

}
