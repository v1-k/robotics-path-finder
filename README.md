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

### Grid Traversal to the Goal
* Once the grid is complete with distances calculated I start with the grid traversal and moving the robot towards the goal.
* Wrote a method that would return the next cell to move to, given the current cell it is at based on the gird.
* As the orientation is important to move the robot towards the next cell. I give the system an initial orientation of the robot in the form of coordinated e.g. (0,1) means facing right/towards increasing x-axis and similarly (1,0) means down/towards increasing y-axis.
* Wrote a method that would take the current orientation, the cell to move to and return an array with 0 index indicating how many cells to move, and index 1 indicating how much to rotate and in which direction(+ve/-ve) e.g. moving from (1,1) to (2,1) with an orientation of (0,1) would mean rotating to the right 90 (-ve) and then moving forward by one cell, so the returning array would look like (1,-90).

### Moving the robot
* Now that I knew which direction to move to and how many steps to move. I wrote methods that would do the required calculations to translate these grid movements to physical distance and movements and then call the c4ev3 APIs to move the robot by the required amount.
* As the distance between our cells was equal to half of the tile width (i.e. 0.305/2). One cell movement meant moving forward by 0.305/2 meters.
* I was keeping track of the distance based on the perimeter of our tire and the degree rotation of the motor. In a 360-degree rotation our robot covered a distance of 2𝜋𝑅 where R is the radius of our tire, so the distance covered by our robot in ∅ degree rotation is 2𝜋𝑅∅/360 . Based on this I moved the robot forward and continuously checked for distance travelled based on the motor rotation and once the distance travelled was equal to the required distance we stopped and after stopping checked for any extra rotations due to inertia and corrected for if any.
* As making a turn around a single tire also meant that I covered a distance of r (distance between the tires) in the forward direction I compensated that by subtracting r distance from the next forward move right after a turn in either direction.
* I also added constant values to our motor rotation equations for each motor to compensate for variable friction offered by different surfaces which could be adjusted as per the surface to try and get more accurate. Both motors had separate constants as no 2 motors provide exactly equal synced rotations.
