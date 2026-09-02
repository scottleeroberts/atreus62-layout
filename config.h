#pragma once

// Home-row mod anti-misfire: resolve as tap when both keys are on the same hand.
// Handedness is auto-derived from the atreus62 layout geometry in keyboard.json.
#define CHORDAL_HOLD

// Engage home-row mods after 170ms hold (default 200ms).
#define TAPPING_TERM 170

// Activate mod immediately when another key is tapped during hold (no TAPPING_TERM wait).
#define PERMISSIVE_HOLD

// Disable hold behavior during fast typing bursts (keys within 100ms always tap).
#define FLOW_TAP_TERM 100
