In *steer-to-pose* we implement a differential drive robot (two degrees of freedom) and 
use a simple control law to drive the robot toward a goal pose:

![Trajectory](/steer-to-pose/trajectory.gif)


In *piano-mover* we compute the configuration space for a robot living in (x, y, theta)
space and, rolling our own Dijsktra implementation for planning in voxel space, compute 
a shortest path from start to goal:

![Configuration space](/piano-mover/cspace.gif)
