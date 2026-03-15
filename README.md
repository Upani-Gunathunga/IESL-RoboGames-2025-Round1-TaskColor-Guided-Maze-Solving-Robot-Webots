# 🤖IESL-RoboGames-2025-Round1-TaskColor-Guided-Maze-Solving-Robot-Webots
A simulated autonomous robot built using the Webots platform that navigates through a maze, detects colored checkpoints, and follows a predefined mission sequence using sensor feedback and camera-based color detection.

This project was developed as part of the **RoboGames 2024 – University Category Completion Round**, where the goal was to design a robot capable of navigating an unknown maze and visiting colored walls in a specific order.

The robot starts exploring the maze using **wall-following behavior**, detects the first color it encounters, and then switches to **color-driven navigation logic** to complete the required sequence.

---

## 🎯 Task Objective

The robot must visit colored checkpoints in the following order:

**Red → Yellow → Pink → Brown → Green**

Key requirements:

* The robot should work **from any starting position**
* It must **detect colors using its camera**
* It must **navigate through junctions autonomously**
* The robot **stops once the final goal (Green) is reached**

---

## ⚙️ Technologies Used

* **Webots Robotics Simulator**
* **C programming (Robot Controller)**
* **E-puck robot model**
* Sensors used:

* Distance sensors (front, left, right)
* Camera for color detection
* Differential drive motor control

---

## 🧠 How the Robot Works

The robot operates in **two main navigation modes**.

### 1️⃣ Exploration Mode — Wall Following

Before detecting any target color, the robot navigates the maze using a **left-wall following algorithm**.

This helps the robot explore the environment without prior knowledge of the maze layout.

The logic is simple:

* If there is a wall in front → turn right
* If the left side is clear → move forward
* If the robot gets too close to a corner → adjust direction

---

### 2️⃣ Color-Based Navigation Mode

Once the **first color is detected**, the robot switches to **goal-based navigation**.

Each color corresponds to a specific **junction turn sequence** that guides the robot through the maze.

Example logic:

* Detect current color using the camera
* Identify the junction using distance sensors
* Execute the next turn in the predefined path sequence
* Continue until the next checkpoint color is found

Each color has its own navigation routine such as:

* `pink_to_red()`
* `yellow_to_red()`
* `brown_to_red()`
* `green_to_red()`
* `red_to_green()`

These functions guide the robot through different maze segments using **predefined turning sequences at junctions**.

---

## 🎨 Color Detection

The robot uses its camera to analyze the average **RGB values** of the image.

Based on these values, the program classifies colors as:

* 🔴 Red
* 🟡 Yellow
* 🩷 Pink
* 🟤 Brown
* 🟢 Green

Once a color is detected, the robot pauses briefly and switches to the corresponding navigation strategy.

---

## 🧭 Junction Detection

Junctions are detected using **three distance sensors**:

* Left
* Front
* Right

These sensor values are converted into a binary pattern describing whether a path exists in each direction.

Example:

| Sensor State | Meaning              |
| ------------ | -------------------- |
| (1,0,0)      | Only left path open  |
| (0,1,1)      | Front and right open |
| (1,1,1)      | Three-way junction   |

The robot uses this information to decide when to execute the **next turn in its navigation sequence**.

---

## 🎥 Demonstration Video

[Watch the demo video](Navigation_VideoDEMO.mp4)


---


## 📸 Simulation Environment

The arena follows the competition specifications:

* **Grid size:** 2.5m × 2.5m
* **Wall spacing:** 0.25m
* **Colored checkpoints:**

  * Red
  * Yellow
  * Pink
  * Brown
  * Green

---

## 👥 Authors

Developed collaboratively by:

* **Upani Ayodya**
* **Piyumi Nipunika**

---

