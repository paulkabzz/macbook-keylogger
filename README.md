# Macbook Keylogger (Educational Use Only)

## Overview

This is a very basic exampple of a **MacBook** keylogger created strictly for educational purposes. It demonstrates how keyloggers work to help people understand system vulnerabilities and defensive techniques.

**⚠️ Important**: This tool should only be used on systems you own or have explicit permission to test. Unauthorised use may violate laws and ethical guidelines.

## Disclaimer
This software is provided for educational purposes only. I assume no liability for any misuse of this tool. Users are solely responsible for ensuring they have proper authorisation before use and for complying with all applicable laws.


## Prerequisites:
Make sure you have the following in order to successfully run the application:
- Macbook
- gcc
- Git

## Installation:

1. Clone the repository:

```bash
git clone git@github.com:paulkabzz/macbook-keylogger.git .
```

2. Compile the program:

```bash
gcc keylogger.c -o keylogger -framework CoreGraphics -framework Carbon -framework ApplicationServices
```

3. Make the executable <i>executable</i>?

```bash
chmod +x keylogger
```

## Permission Configuration

### Accessibility Permissions (Required)

1. Open **System Settings → Privacy & Security → Accessibility**

2. Click the **+** button

3. Navigate to the keylogger directory

4. Select the compiled `keylogger` executable

5. Toggle the permission switch to enable access

<em>Note</em>: You may need to restart the application after granting permissions.


## Usage

### Running the Keylogger:
```bash
./keylogger
```

### Output:
The program will create a `keystrokes.log` file in the same directory, recording keyboard activity.

### Stopping the Keylogger:
Press `Control+C` in the terminal where the keylogger is running.


## Educational Purpose Statement
This tool is designed for:
- Cybersecurity education
- Ethical hacking demonstrations
- Understanding macOS security mechanisms

### Important Considerations:
- Always get proper permission before testing
- Do not use this on systems you don't own
- Be aware of local laws regarding monitoring software
- Use only in controlled, educational environments

## Security Best Practices
For protection against such tools:
- Regularly check Accessibility permissions
- Monitor running processes
- Use security software that detects keyloggers
- Keep your machine updated

---
**Happy Hacking**