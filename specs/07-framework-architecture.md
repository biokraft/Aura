# Framework Architecture Specification

## 1. Purpose

This document specifies the architecture for transforming the single-purpose Aura weather widget into a general-purpose, remotely configurable display framework. This framework will support multiple "Display Agents" (mini-apps) that can be switched on the device.

## 2. Core Concepts

### 2.1. Display Agent

A "Display Agent" is a self-contained module responsible for a specific function, such as displaying weather, a bus schedule, or a calendar. Each agent manages its own:
-   UI layout and rendering (using LVGL).
-   Data fetching and processing.
-   Event handling.

### 2.2. Remote Configuration

The device's behavior will be controlled by a central JSON configuration file hosted on a user-defined URL.

-   **Mechanism:** The device will periodically poll this HTTP endpoint to fetch the configuration.
-   **Configuration URL:** This URL will be a new setting configured during the initial Wi-Fi setup via the captive portal.
-   **Default Behavior:** If the configuration URL is not set or fails to load, the device will default to running the original Weather Agent with its last-known settings.

### 2.3. On-Device Agent Switching

Users can cycle through the configured Display Agents directly on the device.

-   **Trigger:** A long-press gesture on the screen.
-   **Behavior:** Each long-press will deactivate the current agent and activate the next one in the list defined by the remote configuration file. The list will loop back to the beginning.

## 3. Remote Configuration JSON Structure

The JSON file must conform to the following structure:

```json
{
  "version": 1,
  "config": {
    "refresh_interval_seconds": 900
  },
  "agents": [
    {
      "type": "weather",
      "params": {
        "latitude": "48.1351",
        "longitude": "11.5820",
        "location_name": "Munich"
      }
    },
    {
      "type": "bus_schedule",
      "params": {
        "station_name": "Togostraße",
        "line_number": "185",
        "directions": ["Trudering", "Berg am Laim"]
      }
    }
  ]
}
```

-   **`version`**: The version of the configuration schema.
-   **`config.refresh_interval_seconds`**: How often the device should re-fetch this configuration file.
-   **`agents`**: An array of Display Agent objects. The order in this array defines the switching order on the device.
    -   **`type`**: A string identifying the agent to be loaded (e.g., `weather`, `bus_schedule`).
    -   **`params`**: An object containing the specific configuration for that agent.

## 4. Firmware Architecture Refactoring

The existing `weather.ino` will be refactored into a modular, agent-based architecture.

1.  **Agent Manager:** A central component responsible for:
    -   Fetching and parsing the remote configuration JSON.
    -   Storing the list of configured agents.
    -   Managing the active agent and the switching lifecycle (`load`, `run`, `unload`).
    -   Handling the long-press gesture for switching.

2.  **Display Agent Interface:** A standardized C++ interface or base class that all agents must implement.
    ```cpp
    class DisplayAgent {
    public:
        virtual ~DisplayAgent() {}
        // Called once when the agent is first activated
        virtual void setup(const JsonObject& params) = 0;
        // Called repeatedly in the main loop to handle updates
        virtual void loop() = 0;
        // Called when the agent is switched out
        virtual void teardown() = 0;
    };
    ```

3.  **`WeatherAgent`:** The logic from the current `weather.ino` will be encapsulated into a `WeatherAgent` class that implements the `DisplayAgent` interface.

4.  **Main Sketch (`aura.ino`):** The main `.ino` file will be simplified to:
    -   Initialize hardware and Wi-Fi.
    -   Prompt for the new "Configuration URL" setting in WiFiManager.
    -   Initialize the Agent Manager.
    -   In the main `loop()`, call the `loop()` method of the currently active agent. 