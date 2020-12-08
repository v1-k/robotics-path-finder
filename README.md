# Path Planning Robot

## Software Design
### Grid Creation
* I choose potential path planning using Manhattan distance.
* First, created a grid based of the length and width of the obstacle course in this case it was 3.05m X 3.66m. As each tile was .305m wide and long this meant that the course had 10 X 12 tiles. Divided each tile into 4 parts as the obstacles were placed on the intersection of 4 tiles and were tile wide and tile long, so dividing the tile into 4 parts allowed us to label the whole cell as occupied by the obstacle. So, this led to a grid of 20 X 24 in this case but that would totally be dynamic based on the length and width of the obstacle course. The grid is initialized with a high initial value of m*n where n is the number of rows in our gird and m is the number of columns in our grid.
* Then mark the obstacles on this grid based on the coordinates given to us by converting these coordinates to the grid cells by dividing the coordinates by (.305/2) (as dividing each tile in 4 parts)
* Now once I get a cell for an obstacle, mark that cell with a very high number that is n*m*100 where n is the number of rows in our gird and m is the number of columns in our grid. If the cell location that I got is (i,j) where I is the row number and j is the column number then I also need to mark the cells (i-1,j-
1), (i-1,j), (I,j-1) as the obstacle is equivalent to 4 of our grid cells.
* Now to be more fault tolerant and as the minimum distance for the path is 2 tiles wide, also add padding to the obstacles and mark one cell on all the sides of the obstacle as part of the obstacle.

### Distance Calculation
* Once the grid is marked with obstacles, then start with calculating the distance of the cells from the goal using breadth first search approach and level order traversal.
* So I add the goal location in our queue and then traverse each cell in the queue and check its neighbors if they have a value greater that 1+value of the current cell then I replace the neighbors value with this new value and add this neighbor cell to the queue to visit in the next level.
* Once all the cells that are reachable from the goal are travelled, they will be marked by the Manhattan distance indicating how many cells away is the goal from any cell location.
