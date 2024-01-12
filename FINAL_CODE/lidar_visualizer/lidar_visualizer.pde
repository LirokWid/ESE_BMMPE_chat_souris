import processing.serial.*;

int degrees_nb = 360;
int averageDataSize = 60;
int smpl_per_average = degrees_nb/averageDataSize;

int target_angle;
int target_distance; 


int[] lidarData = new int[360];
int[] averageData = new int[averageDataSize];  // Array to store new data

int hoverIndex = -1;
float scaleFactor;
float viewingDistance = 160; // Initial viewing distance


void setup() {
  size(1080, 1200);
  initializeLidarData();
  String portName = "COM10";
  Serial myPort   = new Serial(this, portName      , 460800);
  //Serial myPort = new Serial(this, Serial.list[1], 460800);
}

void draw() {
  background(255);
  updateScaleFactor();
  drawLidar();
  //drawLabels();
  //drawHoverInfo();
  //drawTrigonometricCircle();
}

void serialEvent(Serial myPort) {
  String data = myPort.readStringUntil('\n');
  if (data != null) {
    processData(data);
  }
}

void initializeLidarData() {
  for (int i = 0; i < lidarData.length; i++) {
    lidarData[i] = -1;
  }
}

void processData(String data) {
  String[] values = data.split("\\|");
  for (String value : values) {
    String[] parts = value.trim().split(":");
    if (parts.length == 2) {
      String label = parts[0].trim();
      int distance = Integer.parseInt(parts[1].trim());
      if (label.startsWith("A")) {
        int index = Integer.parseInt(label.substring(1));
        averageData[index] = distance;
        println("(A" + index + "):" + distance);
      } else if (label.startsWith("S")) {
        int index = Integer.parseInt(label.substring(1));
        lidarData[index] = distance;
        //println("(S" + index + "):" + distance);
      } else if (label.startsWith("TA")) {
        target_angle = distance;
        println("(TA):" + target_angle);
      } else if (label.startsWith("TD")) {
        target_distance = distance;
        println("(TD):" + target_distance);
      }
  }
}
}

void updateScaleFactor() {
  float maxDistance = max(lidarData);
  scaleFactor = maxDistance > 0 ? min(width, height) / (2 * maxDistance) : 1.0;
}

void drawLidar() {
  translate(width / 2, height / 2);
  scale(scaleFactor);
  stroke(0);
  drawPoints();
  drawAverage();
  drawTarget();
  noFill();  // Reset fill to default
}

void drawPoints(){
  strokeWeight(5 / scaleFactor);
  for (int i = 0; i < lidarData.length; i++) {
    float angle = radians(i);
    float x = lidarData[i] * cos(angle);
    float y = lidarData[i] * sin(angle);
    if (lidarData[i] != -1) {
      point(x, y);
    }
  }
}
void drawAverage(){
  strokeWeight(1);
  fill(255, 0, 0);  // Red color
  for (int i = 0; i < averageData.length; i++) {
    float angle = radians(i * smpl_per_average);
    float x = averageData[i] * cos(angle);
    float y = averageData[i] * sin(angle);
    if (averageData[i] != 0) {  // Only draw if the data is available
      ellipse(x, y, 30, 30);  // Adjust size as needed
    }
  }
}
void drawTarget(){
  stroke(2);
  strokeWeight(2);
  fill(0, 255, 0);  // Red color
  float angle = radians(target_angle);
  float x = target_distance * cos(angle);
  float y = target_distance * sin(angle);
  line(-width*2, y, width*2, y);
  // Draw vertical line
  line(x, -height*2, x, height*2);  // Draw vertical line
}


void drawTrigonometricCircle() {
  noFill();
  stroke(150);
  ellipse(0, 0, 2 * scaleFactor * viewingDistance, 2 * scaleFactor * viewingDistance);
}

void drawLabels() {
  fill(0);
  textSize(12 / scaleFactor);
  textAlign(CENTER, CENTER);
  for (int i = 0; i < 360; i += 10) {
    float angle = radians(i);
    float x = (viewingDistance + 20) * cos(angle);
    float y = (viewingDistance + 20) * sin(angle);
    text(str(i), x, y);
  }
}

void drawHoverInfo() {
  fill(0);
  textSize(14 / scaleFactor);
  textAlign(LEFT, BOTTOM);
  if (hoverIndex != -1 && hoverIndex < lidarData.length && lidarData[hoverIndex] != -1) {
    float angle = radians(hoverIndex);
    float x = lidarData[hoverIndex] * cos(angle);
    float y = lidarData[hoverIndex] * sin(angle);
    text("Hover: (" + nf(x, 0, 2) + ", " + nf(y, 0, 2) + ")", 20, height - 20);
  }
}

void mouseMoved() {
  hoverIndex = getClosestPointIndex(mouseX - width / 2, mouseY - height / 2);
}

int getClosestPointIndex(float mx, float my) {
  float minDist = Float.MAX_VALUE;
  int index = -1;

  for (int i = 0; i < lidarData.length; i++) {
    float angle = radians(i);
    float x = lidarData[i] * cos(angle);
    float y = lidarData[i] * sin(angle);
    float d = dist(mx, my, x, y);

    if (d < minDist) {
      minDist = d;
      index = i;
    }
  }

  return index;
}
