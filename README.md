# GAMS extrinsic functions example
## Optimum pressure ratio of two-stage compressor train with intercooling

More to come...

We need to add ```-arch x86_64``` flag when working on m1 (arm64 architecture).

```bash
gcc -fPIC -shared -o libintercooler64.dylib intercoolercclib.c intercoolercclibql.c  libCoolProp.dylib -lm -arch x86_64
```

We need to include ```stdbool.h``` header into ```CoolPropLib.h```

```c
#include <stdbool.h>
```
