# SMART-CLASSROOM-AUTOMATION-

## 📖 Overview

The Smart Classroom Automation System is an IoT-based project developed using ESP32 to automate classroom activities and improve classroom management. The system displays real-time schedules, attendance reminders, motivational quotes, break notifications, and important announcements using an LED Matrix Display. It also provides buzzer alerts for attendance, period changes, and special events.

This project aims to reduce manual classroom management tasks and create a smarter learning environment.

---

## 🚀 Features

✅ Real-Time Clock Display

✅ Automatic Period Schedule Display

✅ Attendance Reminder Alerts

✅ Period End Notifications

✅ Morning Motivational Quote Display

✅ Short Break and Lunch Break Alerts

✅ End-of-Day Message

✅ Firebase-Based Remote Message Updates

✅ LED Matrix Information Display

✅ Buzzer Alert System

✅ ESP32 Wi-Fi Connectivity

---

## 🛠 Hardware Components

| Component | Quantity |
|------------|-----------|
| ESP32 Development Board | 1 |
| LED Matrix Display | 1 |
| RTC Module (DS3231) | 1 |
| Buzzer | 1 |
| Jumper Wires | As Required |
| Breadboard | 1 |
| Power Supply | 1 |

---

## 💻 Software Requirements

- Arduino IDE
- Firebase
- ESP32 Board Package
- Wokwi Simulator
- GitHub

---

## 📊 System Architecture

Class Schedule → Firebase → ESP32 → LED Matrix Display

RTC Module → ESP32 → Real-Time Timetable

ESP32 → Buzzer → Attendance & Period Alerts

---

## ⚙️ Working Principle

1. The RTC module continuously provides real-time information.
2. ESP32 checks the current time.
3. Based on the timetable, the corresponding subject name is displayed.
4. Before the end of each period, an attendance reminder buzzer is activated.
5. At the end of each period, a buzzer alert is generated.
6. During breaks and lunch hours, appropriate messages are displayed.
7. Firebase allows administrators to remotely update messages and announcements.
8. Motivational quotes are displayed at the beginning of the day.

---

## 🔔 Alert Functions

### Attendance Alert
- Triggered before the end of each period.
- Buzzer ON for 2 seconds.

### Period End Alert
- Triggered at the end of each class.
- Buzzer ON for 5 seconds.

### Break Notification
- Displays break messages automatically.

### Lunch Notification
- Displays lunch break information.

---

## 🕒 Example Daily Schedule

| Time | Event |
|--------|--------|
| 09:00 AM | Motivational Quote |
| 09:00 AM - 09:50 AM | Period 1 |
| 09:50 AM - 10:40 AM | Period 2 |
| 10:40 AM - 10:50 AM | Morning Break |
| 10:50 AM - 11:40 AM | Period 3 |
| 11:40 AM - 12:30 PM | Period 4 |
| 12:30 PM - 01:30 PM | Lunch Break |
| 01:30 PM - 02:20 PM | Period 5 |
| 02:20 PM - 03:10 PM | Period 6 |
| 03:10 PM - 04:00 PM | Period 7 |
| 04:00 PM - 04:50 PM | Period 8 |
| 04:50 PM | End of Day Message |

---

## 🌐 Firebase Integration

Firebase is used for:

- Displaying announcements
- Updating classroom messages remotely
- Sending motivational quotes
- Managing dynamic display content

---

## 📷 Project Images

### Hardware Setup
(Add Image Here)

### Circuit Diagram
(Add Circuit Diagram Here)

### LED Matrix Output
(Add Output Screenshot Here)

---

## 🔗 Simulation

Wokwi Simulation Link:

(Add Your Wokwi Link Here)

---

## 📂 Source Code

Main Program:

```
SmartClassroom.ino
```

---

## 📈 Future Enhancements

- Face Recognition Attendance
- AI-Based Student Analytics
- Voice Assistant Integration
- Mobile Application Support
- Cloud-Based Monitoring Dashboard
- Automatic Classroom Environment Control

---

## 🎯 Applications

- Schools
- Colleges
- Training Centers
- Smart Educational Institutions

---

## 🏆 Project Outcomes

- Improved classroom management
- Reduced manual effort
- Real-time information display
- Better student awareness
- Smart IoT-based automation

 ## 🧠author 
            A SHAMKUMAR
Electronics and Communication Engineering

Government College of Engineering, Bargur

Skills:
- Embedded Systems
- ESP32 Development
- IoT
- Firebase
- Arduino Programming
- Electronics Design


