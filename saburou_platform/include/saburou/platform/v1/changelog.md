**Date:** 2026-02-19

## Changelog Notice & Versioning Correction

We would like to acknowledge that `CHANGELOG.txt` was not kept up to date during the initial publication phase
of the project. This was an oversight on our side while getting accustomed to the GitHub workflow and release
hygiene. Since the project does not yet provide distributed releases or packaged downloads, the impact has been
minimal, but we recognize that this is not a good habit and we will begin maintaining the changelog properly
from this point forward.

---

## Version Status Update

**saburou-platform v1 is now considered sunset.** **v2 is introduced immediately as its successor.**

This is not a feature-driven change, but a **corrective API break** required to fix an error in the public
macro surface of v1.

### The Issue

Compiler detection in v1 exposed the following macros without version scoping:

- `MAJOR`
- `MINOR`
- `PATCH`
- `VERSION`

These identifiers are:

- Too generic
- Not prefixed with the API version
- Prone to collisions with user code or other libraries
- In violation of our own namespacing guarantees

Because these macros are part of the public contract, **they cannot be safely renamed in-place** without
risking silent ODR and build inconsistencies.

---

## Why This Cannot Be Fixed Inside v1

Our architecture is governed by:

### (ADR-1) Physical Structure and Integrity (ODR Guard)

> Each API version lives in a physically isolated directory (`v1/`, `v2/`, …). Behavior changes that alter the
> exposed contract must not be retrofitted into an existing version.

Modifying those macros inside `v1` would:

- Break the One Definition Rule guarantees we explicitly enforce
- Allow mixed translation units to observe different interfaces
- Violate the isolation model that prevents hybrid builds

In short: **Fixing it inside v1 would contradict the very ADR designed to keep builds safe.**

---

## Support Policy Going Forward

We will follow the project's version lifecycle rules:

- **N (current)** → `v2`
- **N-1** → maintained
- **N-2** → still supported (security / compatibility fixes only)
- **N-3** → enters a **6-month grace period**, then removed

Therefore:

- `v1` is **deprecated immediately**
- It remains available under the **N-2 support window**
- Once it reaches N-3, it will receive **6 additional months of visibility** before archival

Deprecation warnings will be added progressively to guide migration.

---

## Migration Guidance

Users should transition to `v2` as soon as practical. The change is strictly about macro hygiene and versioned
symbol exposure; no platform-detection capability is being removed.

---

## Closing Note

This is admittedly an annoying correction to make so early, but leaving the identifiers as they are would
create far worse integration problems later. Addressing it now keeps the long-term ABI and build model
consistent with the design goals of the project.

Thank you for your patience while the project infrastructure matures.
