#include "header.h"

// Prototype of functions
void startOpenGL();
void *runFoodThread(void *);
void clean_food_off();
void *runThread(void *);
void timer(int );
void display();
void printTimer();
void addText(char *, int , int );
void drawTable();
void drawLineFromToWithWidth(float , float , float , float , float );
void drawAnts(Ant , float);
void drawFood(float , float );
void drawTriangle(int , int , int , int );
void drawRectangle(int , int , int , int );
void reshape(int , int );
void drawBorder(int , int );
void updateAnt(Ant *);
void ant_move(Ant *);
int find_index_ant(float , float );
int find_index(float , float );
float calculateDistance(float , float , float , float );
void generateFood();
int find_index_food();
void initializeAnts();
int getRandomInt(int , int );
float getRandomFloat(float , float );
void cleanBeforeExit();
void newTimer(int );
void newKeyboard(unsigned char , int , int );
void newDisplay();
void startNewOpenGl();

int main(int argc, char **argv)
{
    // read constants from file
    char *filename;
    if (argc > 1)
    {
        filename = argv[1];
        printf("filename: %s\n", filename);
    }
    else
    {
        filename = "arguments.txt";
    }

    readConstants(filename);

    // calculate max foods
    MAX_FOODS = SIMULATION_DURATION * 60 / FOOD_GENERATION_INTERVAL;

    // allocate memory for ants and foods arrays
    ants = (Ant *)malloc(ANT_COUNT * sizeof(Ant));
    foods = (Food *)malloc(MAX_FOODS * sizeof(Food));
    foodMutex = (pthread_mutex_t *)malloc(MAX_FOODS * sizeof(pthread_mutex_t));
    // initialize mutexes
    for(int i = 0; i < MAX_FOODS; ++i)
    {
        pthread_mutex_init(&foodMutex[i], NULL);
    }
    antThreads = (pthread_t *)malloc(ANT_COUNT * sizeof(pthread_t));
    initializeAnts();

    for (int i = 0; i < MAX_FOODS; i++)
    {
        foods[i].on = 0;
    }

    // Create threads for ants
    for (int i = 0; i < ANT_COUNT; i++)
    {
        pthread_create(&antThreads[i], NULL, runThread, (void *)&i);
        usleep(50000);
    }

    // Create a thread for food generation
    pthread_create(&foodThread, NULL, runFoodThread, NULL);

    srand(time(NULL));
    glutInit(&argc, argv);
    // Initialize the food generation timer and the start time
    foodGenerationTimer = time(NULL);
    startTime = time(NULL);
    // calculate the simulation end time in seconds
    simulationEndTime = SIMULATION_DURATION * 60.0;
    movingAnt.x = 200;
    movingAnt.y = 200;
    generateFood();
    startOpenGL();
    return 0;
}

void startOpenGL()
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Ant Searching for Food Simulation");
    glClearColor(238.0 / 255.0, 227.0 / 255.0, 203.0 / 255.0, 1.0);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
}

void drawBorder(int width, int height)
{
    // Set the color and line width for the border
    glColor3f(0.5, 0.25, 0.0); // Brown color for the border
    glLineWidth(10.0);         // Increase line width for the border

    // Draw the border
    glBegin(GL_LINE_LOOP);
    glVertex2i(0, 0);
    glVertex2i(width, 0);
    glVertex2i(width, height);
    glVertex2i(0, height);
    glEnd();

    // draw a line to separate the border and the game area at y = 100
    glBegin(GL_LINES);
    glVertex2i(0, 100);
    glVertex2i(width, 100);
    glEnd();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 800.0, 0.0, 600.0);
    glMatrixMode(GL_MODELVIEW);
}

void drawRectangle(int x, int y, int width, int height)
{
    glBegin(GL_POLYGON);
    glVertex2i(x, y);
    glVertex2i(x + width, y);
    glVertex2i(x + width, y + height);
    glVertex2i(x, y + height);
    glEnd();
}

void drawTriangle(int x, int y, int width, int height)
{
    glBegin(GL_POLYGON);
    glVertex2i(x, y);
    glVertex2i(x + width, y);
    glVertex2i(x + width / 2, y + height);
    glEnd();
}

void drawFood(float x, float y)
{

    glColor3f(1.0, 1.0, 1.0);
    drawRectangle(1000, 0, 0, 0);

    glColor3f(0.0, 0.0, 0.0);
    drawRectangle(x - 5, y, 30, 10);

    glColor3f(0.6, 0.4, 0.2);

    drawRectangle(x, y + 5, 20, 10);
    glColor3f(0.8, 0.1, 0.1);
    drawTriangle(x + 5, y + 15, 10, 10);

    glFlush();
}

void drawAnts(Ant ant, float scale)
{
    // Retrieve the position and scale from the Ant object
    glPushMatrix();
    glTranslatef(ant.x, ant.y, 0.0f);
    glScalef(scale, scale, 1.0f);

    // Draw the body
    glColor3f(0.0f, 0.0f, 0.0f); // Black color
    glBegin(GL_POLYGON);
    float radiusX = 0.2f;
    float radiusY = 0.4f;
    for (int i = 0; i < 360; i++)
    {
        float angle = i * M_PI / 180.0f;
        float x = 0.8*radiusX * cos(angle);
        float y = radiusY * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();

    // Draw the legs
    glColor3f(0.0f, 0.0f, 0.0f); // Black color
    glLineWidth(3.0f);
    for (float y = -0.20f; y <= 0.35f; y += 0.2f)
    {
        glPushMatrix();
        glTranslatef(0.15f, y, 0.0f);
        glBegin(GL_LINES);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(0.10f, -0.05f);
        glEnd();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-0.15f, y, 0.0f);
        glBegin(GL_LINES);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(-0.10f, -0.05f);
        glEnd();
        glPopMatrix();
    }

    // Draw the head
    glColor3f(1.0f, 0.0f, 0.0f); // Red color
    glPushMatrix();
    glTranslatef(0.0f, 0.4f, 0.0f);
    glutSolidSphere(0.09f, 10, 10);
    glPopMatrix();

    glPopMatrix(); // Restore the original transformation matrix
}

void drawLineFromToWithWidth(float x1, float y1, float x2, float y2, float width)
{
    glColor3f(0.5, 0.25, 0.0); // Brown color for the border
    glLineWidth(width);
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

void drawTable()
{

    drawLineFromToWithWidth(WINDOW_WIDTH, 0, WINDOW_WIDTH, BORDER_START, 5);
    for (int i = 0; i < 3; i++)
    {
        drawLineFromToWithWidth(i * WINDOW_WIDTH / 3, 0, i * WINDOW_WIDTH / 3, BORDER_START, 5);
    }

    for (int i = 0; i < 2; i++)
    {
        drawLineFromToWithWidth(0, i * BORDER_START / 2, WINDOW_WIDTH, i * BORDER_START / 2, 5);
    }
}

void addText(char *text, int x, int y)
{
    glRasterPos2f(x, y);
    int len = strlen(text);
    for (int i = 0; i < len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
}

void printTimer(int x, int y, int textSize)
{
    time_t currentTime = time(NULL);
    double elapsedTime = difftime(currentTime, startTime);
    char timerText[50];
    sprintf(timerText, "Timer: %.0f seconds", elapsedTime);
    glColor3f(0.0, 1.0, 0.0);

    glRasterPos2f(x, y);
    for (int i = 0; timerText[i] != '\0'; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, timerText[i]);
    }
}

void cleanBeforeExit()
{
    // free the allocated memory
    free(ants);
    free(foods);
    free((pthread_t *)antThreads);
    // destroy the mutexes
    pthread_mutex_destroy(&*foodMutex);
    pthread_mutex_destroy(&workMutex);
    // destroy the window and exit
    glutDestroyMenu(glutGetMenu());
    exit(0); // End the simulation
}

// Function to generate a random float value between min and max
float getRandomFloat(float min, float max)
{
    return min + ((float)rand() / RAND_MAX) * (max - min);
}

// Function to generate a random integer value between min and max
int getRandomInt(int min, int max)
{
    return (min + rand() % (max - min + 1));
}

// Function to initialize ants at random locations
void initializeAnts()
{
    for (int i = 0; i < ANT_COUNT; i++)
    {
        ants[i].isLeavingPheromone = false;
        ants[i].index = i;
        ants[i].x = getRandomFloat(5, WINDOW_WIDTH);
        ants[i].y = getRandomFloat(110, WINDOW_HEIGHT);
        ants[i].direction = getRandomFloat(0, 2 * M_PI);
        ants[i].speed = getRandomInt(1, 10);
        ants[i].pheromone = 0;
        ants[i].isSearching = true; // Initialize ants in searching mode
    }
}

// Find the first empty food index
int find_index_food()
{
    for (int i = 0; i < MAX_FOODS; i++)
    {
        if (foods[i].on == 0)
            return i;
    }
}

// Function to generate food at a random location
void generateFood()
{
    int index = find_index_food();

    foods[index].x = getRandomFloat(10, WINDOW_WIDTH - 20);
    foods[index].y = getRandomFloat(110, WINDOW_HEIGHT - 20);
    foods[index].percentage = 1000;
    foods[index].on = 1;
}

// Function to calculate the distance between two points
float calculateDistance(float x1, float y1, float x2, float y2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    return sqrt(dx * dx + dy * dy);
}

// Function to return the index of the nearest food
int find_index(float my_x, float my_y)
{
    int index = 0;
    float distance = 0;
    float temp = 0;
    for (int i = 0; i < MAX_FOODS; i++)
    {

        if (foods[i].on == 1)
        {
            temp = calculateDistance(foods[i].x, foods[i].y, my_x, my_y);
            if (distance == 0)
            {
                distance = temp;
                index = i;
            }
            else if (temp < distance)
            {
                distance = temp;
                index = i;
            }
        }
    }
    return index;
}

// Function to return the index of the nearest ant
int find_index_ant(float my_x, float my_y)
{
    int index = 0;
    float distance = 0;
    float temp = 0;
    for (int i = 0; i < ANT_COUNT; i++)
    {
        if (ants[i].isLeavingPheromone)
        {
            temp = calculateDistance(ants[i].x, ants[i].y, my_x, my_y);
            if (distance == 0)
            {
                distance = temp;
                index = i;
            }
            else if (temp < distance)
            {
                distance = temp;
                index = i;
            }
        }
    }
    return index;
}

// Function to control the movement of the ants
void ant_move(Ant *ant)
{
    float tempX, tempY;

    tempX = 0.15 * cos(ant->direction) * ant->speed;
    tempY = 0.15 * sin(ant->direction) * ant->speed;
    int back = 25;

    if ((ant->x + tempX) <= 5) // If the ant is going to hit the left border
    {
        ant->x = back;
        ant->direction += M_PI / 4 * (rand() % 2 == 0 ? 1 : -1); // Add 45 degrees randomly CW or CCW
    }
    else if ((ant->x + tempX) >= (WINDOW_WIDTH - 5)) // If the ant is going to hit the right border
    {
        ant->x = WINDOW_WIDTH - back;
        ant->direction += M_PI / 4 * (rand() % 2 == 0 ? 1 : -1); // Add 45 degrees randomly CW or CCW
    }
    else
    {
        ant->x += tempX;
    }

    if ((ant->y + tempY) <= (BORDER_START + 5)) // If the ant is going to hit the bottom border
    {
        ant->y = BORDER_START + back;
        ant->direction += M_PI / 4 * (rand() % 2 == 0 ? 1 : -1); // Add 45 degrees randomly CW or CCW
    }
    else if ((ant->y + tempY) >= (WINDOW_HEIGHT - 5)) // If the ant is going to hit the top border
    {
        ant->y = WINDOW_HEIGHT - back;
        ant->direction += M_PI / 4 * (rand() % 2 == 0 ? 1 : -1); // Add 45 degrees randomly CW or CCW
    }
    else
    {
        ant->y += tempY;
    }
    // Check if the ant hits the window boundaries
    if (ant->x < 0 || ant->x > WINDOW_WIDTH || ant->y < 0 || ant->y > WINDOW_HEIGHT || ant->y < 105)
    {
        ant->direction += M_PI / 4 * (rand() % 2 == 0 ? 1 : -1); // Add 45 degrees randomly CW or CCW
    }
}

// Function to update an individual ant's behavior
void updateAnt(Ant *ant)
{
    // printing x and y coordinates of the ant
    pthread_mutex_lock(&workMutex);
    int index = find_index(ant->x, ant->y); // Find the nearest food
    int Pindex = find_index_ant(ant->x, ant->y); // Find the nearest ant
    pthread_mutex_unlock(&workMutex);

    float phormoneDistance = calculateDistance(ant->x, ant->y, ants[Pindex].x, ants[Pindex].y); // Find the distance to the nearest ant
    float foodDistance = calculateDistance(ant->x, ant->y, foods[index].x, foods[index].y); // Find the distance to the nearest food

    if (foodDistance <= FOOD_SMELL_DISTANCE) // If the ant is close enough to the food to eat
    {
        pthread_mutex_lock(&foodMutex[index]);
        foods[index].percentage -= FOOD_EATING_PERCENTAGE;
        pthread_mutex_unlock(&foodMutex[index]);
    }
    // Move the ant
    if (ant->isSearching && foodDistance <= FOOD_SMELL_DISTANCE)
    {
        ant_move(ant);
        ant->direction = atan2(foods[index].y - ant->y, foods[index].x - ant->x); // Head towards the food
        ant->food_index = index;
        ant->isSearching = false;             // Set the ant to non-searching mode
        ant->isLeavingPheromone = true;       // Set the ant to leave pheromone trail
        ant->pheromone += 1.0 / foodDistance; // Release pheromone based on the inverse of the distance
    }
    // If the ant smells pheromone but the food is near
    else if (ant->isSearching && ants[Pindex].isLeavingPheromone && phormoneDistance <= PHEROMONE_THRESHOLD)
    {
        ant->direction = atan2(foods[ants[Pindex].food_index].y - ant->y, foods[ants[Pindex].food_index].x - ant->x);
        ant->food_index = index;
        ant->isSearching = false;
        ant->isLeavingPheromone = true;
        ant->pheromone += 1.0 / foodDistance;
    }
    // If the ant smells phermone but the ant is too far, then move 5 degrees porpotional to the food that the ant is heading to
    else if(phormoneDistance > PHEROMONE_THRESHOLD && ant->isSearching && ants[Pindex].pheromone > smell_intensety ){
        float deltax = foods[ants[Pindex].food_index].x - ant->x;
        float deltay = foods[ants[Pindex].food_index].y - ant->y;
        float angle = atan2(deltay , deltax);
        if(fmod(ant->direction, 2*M_PI) > angle){
            ant->direction -= 5 * (M_PI/180);
            ant->food_index = index;
            ant->isSearching = false;
            ant->isLeavingPheromone = true;
            ant->pheromone += 1.0 / foodDistance;
        }
        if(fmod(ant->direction, 2*M_PI) <= angle){
            ant->direction += 5 * (M_PI/180);
            ant->food_index = index;
            ant->isSearching = false;
            ant->isLeavingPheromone = true;
            ant->pheromone += 1.0 / foodDistance;
        }  
    }
    else
    {
        // Move the ant away from pheromone and continue moving randomly
        ant_move(ant);
        // Check if there are other ants within the pheromone radius
        int foundAnt = -1;
        float highestPheromone = 0.0;
        ant->isSearching = true;
        // Update ant behavior
        if (ant->isLeavingPheromone)
        {
            // Move the ant along the pheromone trail
            ant_move(ant);

            // Reduce the pheromone level
            ant->pheromone -= PHEROMONE_DECAY_RATE;

            // Check if the ant has finished leaving the pheromone trail
            if (ant->pheromone <= 0)
            {
                ant->isLeavingPheromone = false;
            }
        }
        else
        {
            // Move the ant randomly
            ant_move(ant);
            // Check if the ant smells food
            float distance = calculateDistance(ant->x, ant->y, foods[index].x, foods[index].y);
            if (distance <= FOOD_SMELL_DISTANCE)
            {
                ant->direction = atan2(foods[index].y - ant->y, foods[index].x - ant->x); // Head towards the food
                ant->isSearching = false;
                ant->food_index = index;
                ant->isLeavingPheromone = true;
                ant->pheromone += 1.0 / distance; // Release pheromone based on the inverse of the distance
            }
            else
            {
                ant->direction += getRandomFloat(-M_PI / 4, M_PI / 4); // Randomly change the direction
            }
        }
    }
}

void display()
{
    int index = 0;
    foods[index];
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < ANT_COUNT; i++)
    {
        drawAnts(ants[i],20);
    }

    drawBorder(WINDOW_WIDTH, WINDOW_HEIGHT);

    // dashboard
    glColor3f(0.047, 0.078, 0.310);
    drawRectangle(0, 0, WINDOW_WIDTH, BORDER_START);
    glColor3f(1.0, 0.0, 0.0);
    addText("Ant:", 320, 22);
    Ant keyAnt;
    keyAnt.x = 360;
    keyAnt.y = 27;
    drawAnts(keyAnt, 25);
    addText("Food:", 380, 22);
    drawFood(440, 20);
    drawTable();

    glColor3f(0.0, 1.0, 0.0);
    glPointSize(10.0);
    glBegin(GL_POINTS);

    for (int i = 0; i < MAX_FOODS; i++)
    {
        if (foods[i].on == 1)
        {
            drawFood(foods[i].x, foods[i].y);
        }
    }

    glEnd();

    printTimer(595, 73, 100);
    // text to display ant count
    char antCountText[50];
    sprintf(antCountText, "Ant Count: %d", ANT_COUNT);
    addText(antCountText, 330, 73);
    // text to display simulation duration
    char durationText[50];
    sprintf(durationText, "Duration: %.0lf seconds", simulationEndTime);
    glColor3f(1.0, 0.0, 0.0);
    addText(durationText, 585, 22);
    // text to display food count
    char foodCountText[50];
    sprintf(foodCountText, "Max Food Count: %d", MAX_FOODS);
    glColor3f(1.0, 0.0, 0.0);
    addText(foodCountText, 45, 22);
    // text to display food eaten
    char foodEatenText[50];
    sprintf(foodEatenText, "Food Eaten: %d", foodEaten);
    glColor3f(0.0, 1.0, 0.0);
    addText(foodEatenText, 60, 73);

    glFlush();
    glutSwapBuffers();
}


void newTimer(int value){
    // to add text to the screen
    glutPostRedisplay();
    glutTimerFunc(16, newTimer, 0);
    // check if the pressed key is 'w' or 'W'
    if ((keyPressed == 'w' || keyPressed == 'W') && movingAnt.y < WINDOW_HEIGHT - 20)
    {
        movingAnt.y += 10;
        keyPressed = ' ';
    }
    // check if the pressed key is 's' or 'S'
    else if ((keyPressed == 's' || keyPressed == 'S') && movingAnt.y > 45)
    {
        movingAnt.y -= 10;
        keyPressed = ' ';
    }
    // check if the pressed key is 'a' or 'A'
    else if ((keyPressed == 'a' || keyPressed == 'A') && movingAnt.x > 10)
    {
        movingAnt.x -= 10;
        keyPressed = ' ';
    }
    // check if the pressed key is 'd' or 'D'
    else if ((keyPressed == 'd' || keyPressed == 'D') && movingAnt.x < WINDOW_WIDTH - 10)
    {
        movingAnt.x += 10;
        keyPressed = ' ';
    }
}

void newKeyboard(unsigned char key, int x, int y){
    switch (key)
    {
    case 'f':
    case 'F':
        cleanBeforeExit();
        break;
    case 'w':
    case 'W':
        keyPressed = 'w';
        break;
    case 's':
    case 'S':
        keyPressed = 's';
        break;
    case 'a':
    case 'A':
        keyPressed = 'a';
        break;
    case 'd':
    case 'D':
        keyPressed = 'd';
        break;
     default:
        break;
    }
    glutTimerFunc(0, newTimer, 0);
}

void newDisplay(){
    // to add text to the screen
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);
    // Ant movingAnt;

    drawAnts(movingAnt,40);
    addText("Your are the ant now, go search for the food (press f to pay respect)", 100, 10);
    glFlush();
    glutKeyboardFunc(newKeyboard);
    glutSwapBuffers();

}

void startNewOpenGl(){
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Ant Searching for Food Simulation");
    glClearColor(238.0 / 255.0, 227.0 / 255.0, 203.0 / 255.0, 1.0);

    glutDisplayFunc(newDisplay);
    glutReshapeFunc(reshape);
}

// Function to handle the timer event
void timer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // Update every 16 milliseconds (approximately 60 frames per second)

    // Generate food at regular intervals
    time_t currentTime = time(NULL);
    if (difftime(currentTime, foodGenerationTimer) >= FOOD_GENERATION_INTERVAL)
    {
        foodGenerationTimer = currentTime;
    }

    // Update ant behavior and check simulation end condition
    currentTime = time(NULL);
    if (difftime(currentTime, startTime) >= simulationEndTime && openNewWindow)
    {
        openNewWindow = false;
        startNewOpenGl();
    }
}


void *runThread(void *arg)
{
    int i = *((int *)arg);
    while (1)
    {
        updateAnt(&ants[i]);
        usleep(7000);
    }
}
void clean_food_off()
{
    // this should clean the off foods
    for (int i = 0; i < MAX_FOODS; i++)
    {

        if (foods[i].on == 1 && foods[i].percentage <= 0)
        {
            foods[i].x = 10000;
            foods[i].y = 10000;
            foods[i].on = 0;
            foodEaten++;
        }
    }
}
void *runFoodThread(void *arg)
{
    while (1)
    {
        clean_food_off();
        generateFood();
        sleep(FOOD_GENERATION_INTERVAL);
    }
}
