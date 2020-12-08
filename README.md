# Path Planning Robot


## Grid Creation:
#### Robotics 5634 Project 1 Group-3
    * We choose potential path planning using Manhattan distance.
    * First, we created a grid based of the length and width of the obstacle course in this case it was 3.05m X
      3.66m. As each tile was .305m wide and long this meant that the course had 10 X 12 tiles. We divided each tile into 4 parts as the obstacles were placed on         the intersection of 4 tiles and were tile wide and tile long, so dividing the tile into 4 parts allowed us to label the whole cell as occupied by the               obstacle. So, this led to a grid of 20 X 24 in this case but that would totally be dynamic based on the length and width of the obstacle course. The grid is         initialized with a high initial value of m*n where n is the number of rows in our gird and m is the number of columns in our grid.
