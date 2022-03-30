# This file specifies properties of the extrinsic trigonometric functions library.
# Using this file, the ql.py script generates C, Delphi, and Fortran90 source
# that implement the querylibrary callback of the extrinsic function library.
#
# The optional [DEFAULT] section overwrites default values from the ql.py script.
# The [Library] section specifies properties of the library in general
# Any section other than [DEFAULT] and [Library] specifies a function that can
# be loaded from the extrinsic function library, its arguments, and other properties.

[DEFAULT]
# The specifications from the DEFAULT section are copied into each section.
# They can be used to overwrite the defaults that are specified in the ql.py script.
#
# The current defaults relevant for the Library section are:
#   Stub           = <name of spec file without .spec>
#   Languages      = 
#   LibraryVersion = 1
#   Vendor         = 
#   NeedLicense    = 0               # library does not need a license
#
# The current defaults relevant for function sections are:
#   Description    =                 # empty string
#   NotInEquation  = 0               # function can be used in equation
#   ZeroRipple     = 0               # function is not necessarily 0 at 0
#   Derivative     = continuous      # function derivative is 'continuous', not 'discontinuous'
#   MaxDerivative  = 2               # highest derivative that can be computed for that function (0, 1, or 2)
#   Arguments      =                 # function has no arguments
#   Exogenous      =                 # no argument is exogenous
#   Endogenous     = __OTHER__       # all arguments other than the exogenous ones are endogenous

# In this example, we change the defaults such that for each function
# an argument with name 'x' is considered as endogenous and an argument
# with name 'MODE' is considered as exogenous.
# As Endogenous is defined to be the special keyword __OTHER__, we could
# also skip the 'Endogenous = x' line, as it is implied by the defaults.
Endogenous = P T s
Exogenous = __OTHER__


[Library]
# We specify a descriptive text for the library. This text is also passed
# through the library and will be displayed in the listing file as a
# result of the $funclibin command.
Description = Test cases for the extrinsic CoolProp library functions
Vendor      = Volkan Akkaya - volkan.akkaya@mu.edu.tr ©2022
Languages   = C

# For other arguments, we leave the default values (see above), i.e.,
# - the Stub for the output files is 'tri'
# - the library and API versions are 1
# - the vendor is the GAMS Development Corp.
# - no license is required to use this library


[Tcrit]
# For each function, we specify its (non-default) properties with such a section.
# The section name must coincide with the section name.
# Here we specify properties of the 'Tcrit' function.

# We specify a descriptive text for the function. This text is also passed
# through the library and will be displayed in the listing file as a
# result of the $funclibin command.
Description = Return the critical temperature of water
MaxDerivative = 0

[Pcrit]
# Same as [Tcrit] section
Description = Return the critical temperature of water
MaxDerivative = 0

[EnthalpyPT]
Description = EnthalpyPT: Returns specific enthalpy of a fluid at a given pressure and temperature
Arguments = P T
MaxDerivative = 2

[EnthalpyPs]
Description = EnthalpyPs: Returns specific enthalpy of a fluid at a given pressure and specific entropy
# We encode this separation between mandatory and optional
# arguments via the '|' sign. That is, all arguments before '|' are
# mandatory and all arguments after '|' are optional.
Arguments = P s
MaxDerivative = 2

[EntropyPT]
Description = EntropyPT: Returns specific entropy of a fluid at a given pressure and temperature
Arguments = P T
MaxDerivative = 2

