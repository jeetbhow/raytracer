import geometry;
import transform;

import <iostream>;

using namespace std;
int main() {
	
  Mat m({
      {{1.0, 0.0, 0.0, 1.0},
       {0.0, 1.0, 0.0, 1.0},
       {0.0, 0.0, 1.0, 1.0},
       {0.0, 0.0, 0.0, 1.0}}
    });

  Pnt3 p(1, 1, 1);
  Pnt3 res = m * p;
}