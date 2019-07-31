Yixing Zheng

zheng129@mail.chapman.edu

Mar 27, 2019

Project 1

**Sudoku**

​    I worked with Zach Granoff, and the result of the project turned out to be pretty well, in my opinion. 

​    We used 3 threads to calculate rows, columns, and grids. The thinking behind was pretty straightforward, we needed 2 threads to do the calculation for rows and columns. Then, the question was, how do we do the calculation for the grid? There are some resources online showing using 27 threads to calculate mostly grids, but we were only allowed to use 3 - 10 thread and I did not see any need for using that many threads. So. we ended up having only 1 thread to do the calculation, and it works fine. 

​    The whole program should be easy to understand since it was written in a form that was combined by smaller components and each component only performed a simple functionality. The disadvantage of writing the code in this form would be a relatively inefficient performance. Moreover, I added 100 and 200 milliseconds to checkColumns() and checkGrids(), respectively, since we had the printing statements inside the checking functions and sometimes threads would interrupt each other causing the printing statement to be incomplete.