/*THIS IMAGE PROCESSOR IS VALID 
 FOR EVERY PGM FILE IN ASCII FORMAT 
CAN PERFORM OPTIONS GIVEN IN THE MENU FILE
 FOR EVERY FUNCTION TO BE WORKING FILES NEEDS TO BE LOADED FIRST
 ALSO ONLY ONE FILE CAN BE LOADED IN THE TERMINAL AT A TIME .
*/
//Required Libraries:
#include <iostream>
#include<fstream>
#include<vector>
#include<cstring>
#include<cmath>
#include<stdlib.h> // library used for opening loaded file in the pgm viewer .
using namespace std;// to avoid using std:: for every input or ouput.
// PROTOTYPE OF ALL FUNCTION ARE DEFINED BELOW: 
struct Image {
    //ALL FEATURES PRESENT IN THE CODE ARE AS FOLLOWS:
    // int loadImage(char ImageName[]);
    // int saveImage(char ImageName[]);
    // void horizontalFlipImage();
    // void verticalFlipImage();
    // void translateImage(int dx, int dy);
    // void brightness_difference(double fact);
    // void negativeImage();
    // void scaleImage(double scaleExtent);
    // void rotate_clockwise();
    // void Image_binary(Image& image);
    // void rotate_Anticlockwise();
    // void median_filter();
    // void image_crop(Image& image);
    // void meanFilter();
    // void rotateArbitraryAngle(Image& image, double angleDegrees);
    // void Change_Image(Image& image);
    // void linearcontraststretching(int maxGrey);
    // void Apply_Derivatives(Image& image);
    // void BONUS_IMAGE_OPENER(const char* imageName);

    // basics to store image values and pgm file protocols 
    char ImageFileName[100];
    vector<vector<int>> ImageData;
    int cols, rows, maxGray;
    vector<char> comment;
    bool imageLoaded;
    bool imageModified;
    // Same protocols to work with second image to use in merging images work.
    char SecondImageFileName[100];
    vector<vector<int>> SecondImageData;
    int secondCols, secondRows, secondMaxGray;
    bool secondImageLoaded;
    bool secondImageModified;

    //Brightness function to enhance or reduce brightness of the given picture 
    void brightness_difference(double fact) {
        //parameter passed as facter to which extent to reduce or enhance brightness.
        for (int r = 0; r < rows; r++)
            for (int c = 0; c < cols; c++) {
                ImageData[r][c] *= fact;
                if (ImageData[r][c] > maxGray)
                    ImageData[r][c] = maxGray;
            }
    }
    // binary function to enhance the white-N-black effect of the image  
    void Image_binary(Image& image) {
        int threshold = 127;
        int maxGray = image.maxGray;

        for (int r = 0; r < image.rows; r++) {
            for (int c = 0; c < image.cols; c++) {
                if (image.ImageData[r][c] <= threshold) {
                    image.ImageData[r][c] = 0;
                }
                else {
                    image.ImageData[r][c] = maxGray;
                }
            }
        }

        image.imageModified = true;
    }

    void Image_contrastStretch() {
        int minpixel = ImageData[0][0];
        int maxpixel = ImageData[0][0];
        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < cols; c++)
            {
                minpixel = min(minpixel, static_cast<int>(ImageData[r][c]));// static_cast used to change data type of ImageData to int.
                maxpixel = min(maxpixel, static_cast<int>(ImageData[r][c]));
            }
        }
        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < cols; c++)
            {
                ImageData[r][c] = static_cast<int>((ImageData[r][c] - minpixel)) * (255.0 / (maxpixel - minpixel));

            }

        }
        imageModified = true;
    }
    
    //function using to load the photo in image  so that it can load its protocol values 
    int loadImage(char ImageName[]) {

        ifstream FCIN(ImageName);

        if (!FCIN.is_open()) {
            cout << "file cannot be opened \n";
            return -1;
        }

        char MagicNumber[5];
        char Comment[100];

        FCIN.getline(MagicNumber, 4);
        FCIN.getline(Comment, 100);
        FCIN >> cols >> rows >> maxGray;

        ImageData.clear();
        ImageData.resize(rows, vector<int>(cols, 0));

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                FCIN >> ImageData[r][c];
            }
        }
        if (FCIN.fail()) {
            cout << "file cannot be converted into required binary \n";
            return -2;
        }

        FCIN.close();
        imageLoaded = true;
        imageModified = false;
        strcpy_s(ImageFileName, ImageName);
        return 0;
    }
    //function to flip the image by horizontal motion 
    void horizontalFlipImage() {
        for (int r = 0; r < rows / 2; r++)   //rows are being half while columns are complete so
                                             // would be less wider columns are complete
            for (int c = 0; c < cols; c++) {
                int T = ImageData[r][c];
                ImageData[r][c] = ImageData[rows - r][c];
                ImageData[rows - r][c] = T;
            }
        return;
    }
    //function to move into or to move out from a picture 
    void scaleImage(double scaleExtent) {
        // passed a scale factor to it to which extent scale it up or down 
        // by a factor of dx and dy 
        int changedwidth = static_cast<int>(cols * scaleExtent);
        int changedheight = static_cast<int>(rows * scaleExtent);

        unsigned int newImageData[355][355];

        for (int r = 0; r < changedheight; r++) {
            for (int c = 0; c < changedwidth; c++) {
                int originalX = static_cast<int>(c / scaleExtent);// static cast is to set default there data type to int  of data type
                int originalY = static_cast<int>(r / scaleExtent);
                newImageData[r][c] = ImageData[originalY][originalX];
            }
        }
        cols = changedwidth;
        rows = changedheight;
        for (int r = 0; r < changedheight; r++) {
            for (int c = 0; c < changedwidth; c++) {
                ImageData[r][c] = newImageData[r][c];
            }
        }

        cout << "Image scaled successfully." << endl;
        imageModified = true;
    }
    //crop function image is passed by reference as parameter.
    void image_crop(Image& image) {
        int staring_row, startC, no_columns, number_rows;
        cout << " Enter starting row of cropped image: ";
        cin >> staring_row;
        cout << " Enter starting col of cropped image: ";
        cin >> startC;
        cout << " Enter number of rows to be cropped: ";
        cin >> no_columns;
        cout << " Enter number of cols to be cropped: ";
        cin >> number_rows;
        //condition to check the values entered and image is loaded successfully .
        if (staring_row < 0 || staring_row >= image.rows || startC < 0 || startC >= image.cols ||
            number_rows <= 0 || no_columns <= 0 || staring_row + number_rows > image.rows || startC + no_columns > image.cols) {
            cout << "Invalid cropping parameters. Please ensure the cropping region is within bounds." << endl;
            return;
        }

        vector<vector<int>> cropdata(number_rows, vector<int>(no_columns, 0));

        for (int r = 0; r < number_rows; ++r) {
            for (int c = 0; c < no_columns; ++c) {
                cropdata[r][c] = image.ImageData[staring_row + r][startC + c];
            }
        }
        image.ImageData = cropdata;
        image.rows = number_rows;
        image.cols = no_columns;

        cout << "Image cropped successfully." << endl;
        image.imageModified = true;
    }
    // arbitrary angle rotation image and angle are passed as parameters.
    void rotateArbitraryAngle(Image& image, double angleDegrees) {
        // Convert angle to radians
        // As  degree_angle * π/180 is the  formula to convert into radian so π/180 =0.0174533.
        double angleRadians = angleDegrees * 0.0174533;//conversion
        double cosTheta = cos(angleRadians);//using trignometric functions 
        double sinTheta = sin(angleRadians);//passing them input angle as their angles 

        // Calculate center of the image
        double centerX = image.cols / 2.0;//by half of image 
        double centerY = image.rows / 2.0;//by half of image 

        // vector secure result 
        vector<vector<int>> temp(image.cols, vector<int>(image.rows, 0));

        for (int i = 0; i < image.rows; ++i) {
            for (int j = 0; j < image.cols; ++j) {
                // Calculate rotated coordinates
                double x = cosTheta * (j - centerX) - sinTheta * (i - centerY) + centerX;
                double y = sinTheta * (j - centerX) + cosTheta * (i - centerY) + centerY;

                // Bilinear interpolation to get the pixel value at the rotated coordinates
                // A resampling method that uses the distance average of four nearest pixels values to estimate a new value
                int x1 = static_cast<int>(floor(x));
                int y1 = static_cast<int>(floor(y));
                int x2 = x1 + 1;
                int y2 = y1 + 1;

                if (x1 >= 0 && x2 < image.cols && y1 >= 0 && y2 < image.rows) {
                    double dx = x - x1;
                    double dy = y - y1;

                    double interpolatedValue = (1 - dx) * (1 - dy) * image.ImageData[y1][x1] +
                        dx * (1 - dy) * image.ImageData[y1][x2] +
                        (1 - dx) * dy * image.ImageData[y2][x1] +
                        dx * dy * image.ImageData[y2][x2];

                    temp[j][i] = static_cast<int>(interpolatedValue);
                }
            }
        }

        // Swap the dimensions and update the image data
        image.ImageData = temp;
        int swap = image.rows;
        image.rows = image.cols;
        image.cols = swap;

        image.imageModified = true;
    }

    //function to translate  the image using dx and dy as its parameters.

    void translateImage(int dx, int dy) {
        //to cut image by a factor
        int temp[297][297];//297 was the limit to save this function from stack overflowing and code crashing 
        for (int r = 0; r < 297; r++) {
            for (int c = 0; c < 297; c++) {
                temp[r][c] = 0;
            }
        }

        for (int r = 0; r < 297; r++) {
            for (int c = 0; c < 297; c++) {
                int row = r + dy;
                int column = c + dx;
                if (row < 297 && row >= 0 && column < 297 && column >= 0) {
                    temp[row][column] = ImageData[r][c];
                }
            }
        }

        for (int i = 0; i < 297; i++) {
            for (int j = 0; j < 297; j++) {
                ImageData[i][j] = temp[i][j];
            }
        }

        imageModified = true;
    }

    void negativeImage() {
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                ImageData[r][c] = maxGray - ImageData[r][c];
            }
        }
        imageModified = true;
    }

    //function to flip image vertically
    void verticalFlipImage() {
        //simple an opposite to horizontal flip where rows are being divided by 2 .
        for (int r = 0; r < rows / 2; r++) {
            for (int c = 0; c < cols; c++) {
                int T = ImageData[r][c];
                ImageData[r][c] = ImageData[rows - 1 - r][c];
                ImageData[rows - 1 - r][c] = T;
            }
        }
    }

    //rotation of image clockwise 
    void rotate_clockwise() {
        //creation of temporary vector
        vector<vector<int>> temp(cols, vector<int>(rows, 0));

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                //swapping orig row with columns and cols with rows -1-index i to rotate clockwise
                temp[j][rows - 1 - i] = ImageData[i][j];
            }
        }
        ImageData = temp;
        int swap = rows;
        rows = cols;
        cols = swap;

        imageModified = true;
    }
    //to blur the image mean filter function is used  
        void meanFilter() {
        int total_matrix = 3;
        int total_matrix_space = total_matrix * total_matrix;
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                int sum = 0;
                for (int i = -total_matrix / 2; i <= total_matrix / 2; i++) {
                    for (int j = -total_matrix / 2; j <= total_matrix / 2; j++) {
                        int vertical_length = r + i;
                        int width = c + j;
                        if (vertical_length >= 0 && vertical_length < rows && width >= 0 && width < cols) {
                            sum += ImageData[vertical_length][width];
                        }
                    }
                }
                ImageData[r][c] = sum / total_matrix_space;
            }
        }
    }

        //median filter to sharp an image 
    void median_filter() {
                   //temp vector 
        vector<vector<int>> copydata = ImageData;

        for (int r = 1; r < rows - 1; ++r) {
            for (int c = 1; c < cols - 1; ++c) {

                int similar[9] = {
                    ImageData[r - 1][c - 1], ImageData[r - 1][c], ImageData[r - 1][c + 1],
                    ImageData[r][c - 1], ImageData[r][c], ImageData[r][c + 1],
                    ImageData[r + 1][c - 1], ImageData[r + 1][c], ImageData[r + 1][c + 1]
                };
                for (int i = 0; i < 5; ++i) {
                    int minIndex = i;
                    for (int j = i + 1; j < 9; ++j) {
                        if (similar[j] < similar[minIndex]) {
                            minIndex = j;
                        }
                    }

                    swap(similar[i], similar[minIndex]);
                }
                int medianValue = similar[4];
                copydata[r][c] = max(0, min(maxGray, medianValue));
            }
        }

        ImageData = copydata;
        imageModified = true;
    }
  //anti clock wise rotatio of an image 
    void rotate_Anticlockwise() {
        // Created a temporary vector to store the rotated values
        vector<vector<int>> temp(cols, vector<int>(rows, 0));
        //almost same as clockwise here rows are change with cols-1-index j and orig cols with rows 
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                temp[cols - 1 - j][i] = ImageData[i][j];
            }
        }
        //swapping is done 
        ImageData = temp;
        int swap = cols;
        cols = rows;
        rows = swap;

        imageModified = true;
    }
  //derivative or sharp edges function of an image 
    //image is passed as reference to the function
    void Apply_Derivatives(Image& image) {//function will punch up characters in the image 
        int kernel[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };// formula to obtain derivative of image 
        // or in other words to blur out the edges.

        vector<vector<int>> edgesData(rows, vector<int>(cols, 0));

        for (int i = 1; i < rows - 1; ++i) {
            for (int j = 1; j < cols - 1; ++j) {
                int sum = 0;

                for (int r = 0; r < 3; ++r) {
                    for (int c = 0; c < 3; ++c) {
                        sum += image.ImageData[i - 1 + r][j - 1 + c] * kernel[r][c];
                    }
                }

                edgesData[i][j] = abs(sum);  // Ensure positive values
            }
        }
        image.ImageData = edgesData;

        cout << "Edges Found Successfully." << endl;
        image.imageModified = true;
    }

    void Change_Image(Image& image) {
        int nHeight, nWidth;// to which extent you want to resize .
        cout << "Enter new height of image: "; cin >> nHeight;
        cout << "Enter new width of image: "; cin >> nWidth;

        //  temporary vector to store the resized values
        vector<vector<int>> resizedData(nHeight, vector<int>(nWidth, 0));

        for (int y = 0; y < nHeight; y++) {
            for (int x = 0; x < nWidth; x++) {
                int originalX = x * image.cols / nWidth;
                int originalY = y * image.rows / nHeight;
                resizedData[y][x] = image.ImageData[originalY][originalX];
            }
        }
        //substitution of image protocols to declared type in function
        image.ImageData = resizedData;
        image.rows = nHeight;
        image.cols = nWidth;
        //comments of success
        cout << "Image resized successfully." << endl;
        image.imageModified = true;
    }
 
    //function to perform linear contrasting 
    void linearcontraststretching(int maxGrey) {
        int minGrey = ImageData[0][0];

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (ImageData[r][c] < minGrey)
                    minGrey = ImageData[r][c];
            }
        }
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                float pixel = static_cast<float>(ImageData[r][c]);
                float result = (pixel - minGrey) / (maxGrey - minGrey) * 255.0f;
                ImageData[r][c] = static_cast<int>(result); 
            }
        }

        imageModified = true;

    }

    void BONUS_IMAGE_OPENER(const char* imageName) {


        const char* commandFormat = "start %s";

        // space for the command line
        char command[256];
        sprintf_s(command, commandFormat, imageName);//

        // command to open the image viewer
        int result = system(command);

        // Check success
        if (result == 0) {
            cout << "Image opened in the default viewer." << endl;
        }
        else {
            cout << "Error opening the image in the viewer." << endl;
        }

    }


    //function to save the called image or the after changes applied.pgm
    int saveImage(char ImageName[]) {
        ofstream FCOUT(ImageName);
        if (!FCOUT.is_open()) {
            cout << "file cannot be saved \n";
            return -1;
        }

        FCOUT << "P2\n# created by Abubakar and owais \n"// pgm format comment 
            << cols << " " << rows << endl << maxGray << endl;
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++)
                FCOUT << ImageData[r][c] << " ";
            FCOUT << endl;
        }
        FCOUT.close();
        imageModified = false;
        return 0;
    }
};


struct Menu {
    vector<string> menuItems;

    Menu(char menuFile[]) {
        loadMenu(menuFile);
    }

    int loadMenu(char menuFile[]) {
        ifstream IN;
        IN.open(menuFile);
        if (!IN.is_open())
            return -1;
        char menuItem[100], TotalItems[10];

        int Choices;

        IN.getline(TotalItems, 8);
        Choices = atoi(TotalItems);
        for (int i = 1; i <= Choices; i++) {
            IN.getline(menuItem, 99);
            menuItems.push_back(menuItem);
        }
        IN.close();
        return Choices;
    }

    int presentMenu() {
        int userChoice;
        int totalChoices = menuItems.size();


        do {
            int k = 1;
            for (int i = 0; i < totalChoices; i++) {
                if (menuItems[i][0] != '*') {
                    cout << k << "\t" << menuItems[i] << endl;
                    k++;
                }
            }
            cout << " Enter Your Choice (1 - " << k - 1 << " ) ";
            cin >> userChoice;
        } while (userChoice < 1 || userChoice > totalChoices);
        return userChoice;
    }

};

int main() {
    // mainmenu file present in project folder
    char MenuFile[] = "mainmenu.txt";
    Image images[2];
    int activeImage = 0;
    int errorCode = 0;
    int userChoice;
    int totalChoices;

    Menu menu(MenuFile);
    totalChoices = menu.menuItems.size();
    do {// use do while loop for the selection of required function.
        userChoice = menu.presentMenu();
        if (1 == userChoice) {
            char ImageFileName[100];
            cout << "Specify File Name ";
            cin >> ImageFileName;
            errorCode = images[activeImage].loadImage(ImageFileName);
            if (errorCode == 0) {
                cout << "File Loaded Successfully " << endl;
            }
            else {
                cout << "Load Error: Code " << errorCode << endl;
            }
        }
        else if (2 == userChoice) {
            char ImageFileName[100];
            cout << "Specify File Name ";
            cin >> ImageFileName;
            errorCode = images[activeImage].saveImage(ImageFileName);
            if (errorCode == 0) {
                cout << "File Saved as " << ImageFileName << endl;
            }
            else {
                cout << "Save Error: Code " << errorCode << endl;
            }
        }
        else if (3 == userChoice) {
            images[activeImage].horizontalFlipImage();
            cout << "you need to save changes \n";
        }
        else if (4 == userChoice) {
            images[activeImage].verticalFlipImage();
            cout << "you need to save changes \n";
        }

        else if (5 == userChoice)
        {
            int x, y;
            cout << "enter dx and dy to translate \n";
            cin >> x >> y;
            images[activeImage].translateImage(x, y);
            cout << "you need to save changes \n";
        }
        else if (6 == userChoice) {
            double extent;
            cout << "enter from 0.1-3 to which extent you want to bright up or low \n";
            cin >> extent;
            images[activeImage].brightness_difference(extent);
            cout << "you need to save changes \n";
        }
        else if (7 == userChoice)
        {
            images[activeImage].negativeImage();
            cout << "you need to save changes \n";
        }
        else if (8 == userChoice) {
            int fact;
            cout << "please enter the factor to  scale your image 0.1-2 \n ";
            cin >> fact;
            images[activeImage].scaleImage(fact);
            cout << "you need to save changes \n";
        }
        else if (9 == userChoice) {
            images[activeImage].rotate_clockwise();
            cout << "you need to save changes \n";
        }
        else if (10 == userChoice) {

            images[activeImage].Image_binary(images[activeImage]);
            cout << "Binary conversion applied." << endl;
            cout << "You need to save changes." << endl;
        }
        else if (11 == userChoice)
        {
            images[activeImage].rotate_Anticlockwise();
            cout << "you need to save changes \n";
        }
        else if (12 == userChoice)
        {
            images[activeImage].median_filter();
            cout << "you need to save changes \n";
        }
        else if (13 == userChoice)
        {
            images[activeImage].image_crop(images[activeImage]);
            cout << "you need to save changes \n";
        }
        else if (14 == userChoice)
        {
            images[activeImage].meanFilter();
        }
        else if (15 == userChoice)
        {
            int angle;
            cout << "please enter your angle to which you want to rotate your picture \n";
            cin >> angle;
            images[activeImage].rotateArbitraryAngle(images[activeImage], angle);
            cout << "you need to save changes \n";
        }
        else if (16 == userChoice)
        {
            images[activeImage].Change_Image(images[activeImage]);
            cout << "you need to save changes \n";

        }
        else if (17 == userChoice)
        {


            int activeImage = 0;
            int maxGrayValue = images[activeImage].maxGray;
            images[activeImage].linearcontraststretching(maxGrayValue);
            cout << "you need to save changes \n";
        }
        else if (18 == userChoice)
        {
            images[activeImage].Apply_Derivatives(images[activeImage]);
            cout << " you need to save changes \n ";

        }
        else if (19 == userChoice)
        {

            images[activeImage].BONUS_IMAGE_OPENER(images[activeImage].ImageFileName);
            cout << " you need to save changes \n ";
        }

    } while (userChoice != totalChoices);
    cout << "THANK-YOU FOR YOU OUR DIGITAL IMAGE PROCESSOR \n";
    return 0;
}