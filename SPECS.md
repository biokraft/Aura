# 📋 Project Specifications

This document serves as the central hub for all technical and functional specifications for the Aura project. Each document details a specific aspect of the project, from hardware to deployment.

## 📂 Specification Documents

| File                               | Description                                                                 |
| ---------------------------------- | --------------------------------------------------------------------------- |
| 📝 [01-hardware.md](specs/01-hardware.md) | Details the hardware components, pinouts, and electrical requirements.      |
| ⚙️ [02-application.md](specs/02-application.md) | Describes the core application logic, states, and data flow.                |
| 🎨 [03-ui-ux.md](specs/03-ui-ux.md) | Outlines the user interface design, screen layouts, and user experience flow. |
| 🛠️ [04-build-and-environment.md](specs/04-build-and-environment.md) | Defines the build process, development environment, and dependencies.         |
| 🚀 [05-deployment.md](specs/05-deployment.md) | Explains the firmware flashing and deployment procedures.                   |
| 📚 [06-documentation.md](specs/06-documentation.md) | Guidelines for project documentation structure and maintenance.             |
| 🏛️ [07-framework-architecture.md](specs/07-framework-architecture.md) | Provides an overview of the software architecture and key frameworks.       |
| 🚌 [08-app-bus-schedule.md](specs/08-app-bus-schedule.md) | Details the application's internal event bus and message schedule.          |
| 📂 [09-source-code-organization.md](specs/09-source-code-organization.md) | Defines the modular source code structure and organization.                 |
| 🪵 [10-logging-strategy.md](specs/10-logging-strategy.md) | Defines the project's logging strategy and framework choice.                |

## 🚀 Implementation Plan

| Phase | Focus Area | Key Deliverables | Related Specs | Status |
| :--- | :--- | :--- | :--- | :--- |
| 1 | Source Code Restructuring | Reorganize the `aura` directory into a modular structure with `src`, `assets`, and `components`. | [09-source-code-organization.md](specs/09-source-code-organization.md) | TBD |
| 2 | Logging System Implementation | Integrate `spdlog` for deferred/asynchronous logging to `Serial`. Create a central logging component and provide logger instances to other components. | [10-logging-strategy.md](specs/10-logging-strategy.md) | TBD |


[⬅️ Back to Home](./README.md) 