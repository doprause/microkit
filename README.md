```
                               oo                            dP       oo   dP
                                                            88            88
                    88d8b.d8b. dP .d8888b. 88d888b. .d8888b. 88  .dP  dP d8888P
                    88'`88'`88 88 88'  `"" 88'  `88 88'  `88 88888"   88   88
                    88  88  88 88 88.  ... 88       88.  .88 88  `8b. 88   88
                    dP  dP  dP dP `88888P' dP       `88888P' dP   `YP dP   dP
```

# microkit

**microkit** is an opinionated software framework for developing embedded systems in C.

## Getting Started

1. Create a new software development project using an IDE suitable for your target platform, e.g. STM32CubeIDE for STM32 microcontrollers or MBLAB for Microchip microcontrollers.
2. Clone the microkit repository as a sub-repository into the project directory. Typically you would use a folder structure like `<project-root>/libs/microkit` and add the microkit repository as a submodule with `git submodule add https://github.com/doprause/microkit.git ./libs/microkit`.
3. Setup the project to include the microkit source files at `<project-root>/libs/microkit/lib/**/*` and also set the include path to `<project-root>/libs/microkit/lib`. This is typically done in the IDE's project settings.
