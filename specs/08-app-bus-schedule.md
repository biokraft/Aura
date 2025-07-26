# App Bus Schedule Specification

[⬅️ Back to Specifications](../SPECS.md)

## 1. Purpose

This document specifies the functional and technical requirements for the "Bus Schedule" Display Agent. This agent is designed to display real-time public transit departure information for a specific bus line and station.

## 2. Functional Requirements

-   The agent must be configurable via the remote JSON configuration as defined in `specs/07-framework-architecture.md`.
-   It shall display departures for a single bus line from a specified station.
-   It must show departures for two distinct, user-configurable directions (e.g., "Trudering" and "Berg am Laim").
-   The display will show a list of the next upcoming departures for each direction.
-   For each departure, it will display:
    -   The scheduled departure time.
    -   The final destination of the run.
    -   Real-time delay information, if provided by the API.
-   The data must be periodically refreshed to ensure timeliness.

## 3. User Interface (UI)

The UI will be a single, non-interactive screen designed for clear readability.

-   **Screen Title:** The top of the screen will display the station name and line number (e.g., "Togostraße - Bus 185").
-   **Layout:** The screen will be divided into two vertical columns, one for each direction.
-   **Column Header:** Each column will be headed by its direction/destination name (e.g., "Direction: Trudering").
-   **Departure List:** Each column will list the next 4-5 upcoming departures.
    -   **Departure Time:** Displayed in a large font (e.g., "14:35").
    -   **Delay:** If a delay is present, it will be shown next to the time in a contrasting color (e.g., "+5" in red).
    -   **Destination:** The final destination of that specific bus run will be displayed below the time in a smaller font.

*UI Mockup:*
```
+------------------------------------------+
|         Togostraße - Bus 185             |
+------------------------------------------+
| Direction: Trudering | Direction: B.a.Laim|
+----------------------+-------------------+
| 14:35  +2            | 14:42             |
|  -> Ostbahnhof      |  -> Westfriedhof  |
|                      |                   |
| 14:55                | 15:02  +1         |
|  -> Ostbahnhof      |  -> Westfriedhof  |
|                      |                   |
| 15:15                | 15:22             |
|  -> Ostbahnhof      |  -> Westfriedhof  |
|                      |                   |
+----------------------+-------------------+
```

## 4. Technical Requirements

### 4.1. Configuration Parameters

The agent will expect the following `params` in the remote configuration JSON:

-   `station_name` (string): The name of the bus station (e.g., "Togostraße").
-   `line_number` (string): The bus line identifier (e.g., "185").
-   `directions` (array of strings): An array containing the two terminal destinations for filtering.

### 4.2. Data Source

-   **API:** The agent will use an unofficial MVG API endpoint for fetching real-time departure data.
-   **Wrapper:** The implementation should be based on a reliable community-maintained wrapper, such as the one found at `github.com/mondbaron/mvg`, which uses the `https://www.mvg.de/api/bgw-pt/v3` endpoint.
-   **Data Flow:**
    1.  On setup, use the `station_name` to find the station's unique ID via the API.
    2.  Periodically (e.g., every 60 seconds), fetch all departures for the found station ID.
    3.  Filter the results to include only those matching the configured `line_number`.
    4.  Group the filtered departures into two lists based on whether their destination matches the strings provided in the `directions` array.
    5.  Update the LVGL UI with the processed data. 