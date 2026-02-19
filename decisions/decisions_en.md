# 📖 saburou-platform: Architecture Decision Records (ADR)

This document records the technical and maintenance decisions of saburou-platform. These rules **prioritize the
user**, ensuring that saburou-platform is powerful, easily usable, and does not stop working from one day to
the next.

---

## 🛠️ Level 1: Usage and Code Philosophy

### (ADR-1) Physical Structure and Integrity (ODR Guard)

Wed 2026-02-18 12:30:44 v1

- **Decision**: Each major version of the API resides in its own physical folder (e.g., `v1/`) with its
  respective umbrella file (`v1.hpp`).
- **Synchronization**: If the internal behavior changes but the API remains, the
  `SABUROU_PLATFORM_VX_CORE_REVISION` macro is incremented.
- **Protection**: Revision sentinels are used that trigger a `#error` if different core revisions are attempted
  to be mixed in a single translation unit.
- **Impact**: Total isolation between versions. It is guaranteed that the linker does not produce hybrid
  behaviors or violations of the One Definition Rule (ODR).

### (ADR-2) Environment Cleanup (Macro Cleanup)

Wed 2026-02-18 12:30:44 v1

- **Context**: Macros predefined by old compilers (such as `linux` or `unix`) contaminate the global space and
  break modern enums or namespaces.
- **Decision**: saburou-platform applies an `#undef` to these macros by default when loading the core.
- **Impact**: Allows using clean and modern names like `os::type_t::linux`. If the user requires these macros
  back for legacy code, the `env/recover.hpp` header is provided.

### (ADR-3) Philosophy of the 99% and Escape Valves

Wed 2026-02-18 12:30:44 v1

- **Decision**: The library is designed to elegantly and automatically cover 99% of common use cases in
  platform detection.
- **Overrides**: The user is not blocked in the remaining 1%. To the extent possible (while viable and making
  sense), versions implement manual _overrides_ (such as defining `SABUROU_PLATFORM_DEFINED = 1`) to disable
  detection. This implies that certain directives will have to be defined by hand under certain builds with
  manual detection by the user + possible feature breakages, but at least some possibility is granted to the
  user during the evolution of the project.

### (ADR-4) Evolution of the C++ Standard

Wed 2026-02-18 12:30:44 v1

- **Decision**: The C++ standard required by saburou-platform is a pragmatic decision based on the utility of
  language features, not an immovable dogma.
- **Impact**: Although C++23 is currently prioritized for power and ergonomics, if in the future it is more
  beneficial to decrease the version to gain scope or increase it to take advantage of new tools, it will be
  done in a communicated manner.

---

## 🛡️ Level 2: Maintenance and Security

### (ADR-5) Security and Community Commitment

Wed 2026-02-18 12:30:44 v1

- **Decision**: Faced with the detection of a critical security flaw, saburou-platform's commitment is to act
  with the maximum possible speed, giving it absolute priority over the development of new features.
- **Realism**: Effective resolution will depend on the technical complexity and available knowledge.
  **Community collaboration is actively encouraged** to audit, report, and patch flaws in a **transparent** and
  professional manner.

### (ADR-6) In-situ Changelog

Wed 2026-02-18 12:30:44 v1

- **Decision**: Each version maintains its history of changes with respect to the previous version, as well as
  core changes in a physical file located at `vX/changelog.txt`.

### (ADR-7) Specialization Branches

Wed 2026-02-18 12:30:44 v1

- **Decision**: saburou-platform does not rule out the creation of dedicated branches or derived projects to
  provide support for old systems or legacy C++ standards (e.g., C++11) if there is a real need. These
  derivations will be managed with their own version tables.

---

## ⏳ Level 3: Life Cycle and Retirement

### (ADR-10) Direct Publication and Production Stability

Thu 2026-02-19 20:10:00 v1

- **Supersedes**: [(ADR-8) Alpha Validation Phase](#adr-8-alpha-validation-phase)
- **Decision**: The mandatory alpha validation period (the `-alpha` suffix) is removed. Major versions may be
  published as `stable` immediately.
- **Rationale**: The 15-60 day lockout period is incompatible with the need for immediate production deployment
  of critical design improvements. Stability is guaranteed through the physical isolation defined in
  **(ADR-1)**.
- **Impact**: Enables agile project evolution without compromising the integrity of previous versions.

### (ADR-9) Support and Transition to Legacy-Archive

Wed 2026-02-18 12:30:44 v1

- **End of Support**: A version stops receiving patches or active maintenance when **BOTH** conditions are met:
    1. There are **3 posterior major versions** available (N-3 Rule).
    2. At least **6 months** have passed since the version reached the previous state.
- **Legacy-Archive**: A version is physically moved to the archive folder (and removed from the main branch)
  **2 years after** having reached its "End of Support".
- **Impact**:
    - A predictable and extended life cycle is guaranteed, protecting users from premature obsolescence even in
      the face of frequent releases of new versions.
    - There will be times when we will be dragging very old code and the number of files in the project becomes
      absurdly large.

---

## 📜 Superseded Decisions History

### (ADR-8) Alpha Validation Phase

Wed 2026-02-18 12:30:44 v1

- **Superseded by**:
  [(ADR-10) Direct Publication and Production Stability](#adr-10-direct-publication-and-production-stability)
- **Decision**: Every new major API version will be published under the `-alpha` suffix (e.g., `v2-alpha`) for
  a period of at least 15 to 60 days.
- **Objective**: To validate the design in real-world environments before declaring the version as `stable` and
  definitively freezing its API, and/or providing the community with the opportunity to test features prior to
  implementation.
- **Assumptions**: Alpha versions may change their API at any time, be incomplete, feature-limited, or even be
  aborted and permanently removed without prior notice or warranty; therefore, the user shall not use them in
  production.
