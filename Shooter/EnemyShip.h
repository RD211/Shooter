#ifndef ENEMYSHIP_H
#define ENEMYSHIP_H
#include <ctime>
#include <random>
#include <windows.h>
class EnemyShip
{
    public:
      COORD coords;
      int direction;
      EnemyShip(int seed)
      {
          srand (seed);
          coords.Y = rand()%10+2;
          coords.X = rand()%105+3;
          direction = rand()%2;
      }
      void AdvanceShip()
      {
          if(direction==1)
          {
              if(coords.X==114)
              {
                  direction=0;
                  return;
              }
              coords.X++;
          }
          else
          {
              if(coords.X==3)
              {
                  direction=1;
                  return;
              }
              coords.X--;
          }
      }
      COORD ShootProjectile()
      {
          COORD ProjectileEnemy;
          ProjectileEnemy.X=coords.X;
          ProjectileEnemy.Y=coords.Y+2;
          return ProjectileEnemy;
      }

};

#endif // ENEMYSHIP_H
