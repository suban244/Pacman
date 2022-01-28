### DFS Pitfall
A issue with using depth first search in a game like pacman, was that the destination keeps on changing. We need to continuously update the path we take. 
We thought that using a new DFS whenever a entity reaches to the center of a block will be a good idea but we soon found out it was not a good idea.
DFS finds **a** path to the destination and the path depends upto order of search. A situation may arrise where we end up with two opposing searches that occour one after the other.
This was the case here. The searching other was set to right, down, left, up.
* When Entity is at the corner, DFS produces a path that starts from a left move, as entity cannot move right or down.
* When Entity is at the left of the corder, becuase of the search order, DFS produces a path that start from a right move.
Thus we have two conflicting movements that occour one after the other.
