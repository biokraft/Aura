# Framework Architecture Specification

[⬅️ Back to Specifications](../SPECS.md)

## 1. Purpose

This document specifies the architecture for transforming the Aura application into a general-purpose, remotely configurable display framework. This framework will support multiple "Display Agents" (mini-apps) that can be switched on the device.

The component-based refactoring detailed in [09-source-code-organization.md](./09-source-code-organization.md) is the foundational first phase of implementing this architecture.

## 2. Core Concepts

### 2.1. Display Agent

A "Display Agent" is a self-contained module responsible for a specific function, such as displaying weather, a bus schedule, or a calendar. Each agent will leverage the core application components (`display`, `ui`) for rendering and will encapsulate its own data-handling logic.

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

The firmware will be refactored from a single-purpose application into a modular, agent-based architecture.

1.  **Agent Manager:** A central component responsible for:
    -   Fetching and parsing the remote configuration JSON.
    -   Storing the list of configured agents.
    -   Managing the active agent and the switching lifecycle (`load`, `run`, `unload`).
    -   Handling the long-press gesture for switching.

2.  **Display Agent Interface:** A standardized C++ interface or base class that all agents must implement. This interface ensures that the Agent Manager can treat all agents uniformly.
    ```cpp
    class DisplayAgent {
    public:
        virtual ~DisplayAgent() {}
        // Called once when the agent is activated.
        virtual void setup(const JsonObject& params) = 0;
        // Called repeatedly to handle updates.
        virtual void loop() = 0;
        // Called when the agent is deactivated.
        virtual void teardown() = 0;
    };
    ```

3.  **Core Components (`display`, `ui`):** The existing `display` and `ui` components will provide services to all agents for rendering graphics and managing UI elements.

4.  **`WeatherAgent`:** The logic from the `weather` component will be encapsulated into a `WeatherAgent` class that implements the `DisplayAgent` interface.

5.  **Main Sketch (`aura.ino`):** The main `.ino` file will be simplified to:
    -   Initialize hardware, core components (`display`, `ui`), and Wi-Fi.
    -   Prompt for the "Configuration URL" setting in WiFiManager.
    -   Initialize the Agent Manager.
    -   In the main `loop()`, call the `loop()` method of the currently active agent. 