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
#   Vendor         = GAMS Development Corporation
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
Exogenous = MODE
Endogenous = x


[Library]
# We specify a descriptive text for the library. This text is also passed
# through the library and will be displayed in the listing file as a
# result of the $funclibin command.
Description = Trigonometric functions Sine and Cosine, Constant Pi

# We want querylibrary implementations in the languages C, Delphi, and Fortran90.
# For C,         the ql.py script generates a file tricclibql.c.
# For Delphi,    the ql.py script generates a file tridclibql.inc.
# For Fortran90, the ql.py script generates a file triifortlibql.f90.
# The prefix 'tri' is taken from the Stub argument, which defaults to 'tri' in this case.
Languages = C Delphi Fortran90

# For other arguments, we leave the default values (see above), i.e.,
# - the Stub for the output files is 'tri'
# - the library and API versions are 1
# - the vendor is the GAMS Development Corp.
# - no license is required to use this library


[SetTriMode]
# For each function, we specify its (non-default) properties with such a section.
# The section name must coincide with the section name.
# Here we specify properties of the 'SetTriMode' function.

# We specify a descriptive text for the function. This text is also passed
# through the library and will be displayed in the listing file as a
# result of the $funclibin command.
Description = Set mode globally, could still be overwritten by mode at (Co)Sine call

# This function has a discontinuous derivative.
Derivative = discontinuous

# This function is not allowed to be used within a GAMS equation, as it
# changes the internal status of the library, see also Test Library model
# trilib04.
NotInEquation = 1

# This function returns the previous mode, not the newly installed
# mode, so its input is independent from its output.  Therefore,
# setting ZeroRipple=0 is proper.  Zero is the default, but we can set
# it anyway.
ZeroRipple = 0

# This function has one argument, which is named 'MODE'.
# Due to our setting in the [DEFAULT] section, it is exogenous.
Arguments = MODE


[Cosine]
# The cosine function computes a cosine, where the argument can be
# specified in radians or degrees. The optional second arguments specifies
# whether radians or degrees is used. If the second argument is not given,
# the mode that is stored internally in the library is used. This mode
# can be set by the SetTriMode function.

Description = Cosine: mode=0 (default) -> radians, mode=1 -> degrees

# The function can be called with 1 or 2 arguments.
# The first argument, x, is mandatory, while the second argument, MODE,
# is optional. We encode this separation between mandatory and optional
# arguments via the '|' sign. That is, all arguments before '|' are
# mandatory and all arguments after '|' are optional.
#
# Due to our settings in the [DEFAULT] section, variable x is endogenous
# and variable MODE is exogenous.
Arguments = x | MODE

[Sine]
# The specification of the Sine is similar to the one for Cosine.

Description = Sine: mode=0 (default) -> radians, mode=1 -> degrees
Arguments = x | MODE

# If the extrinsic does not provide derivatives, we signal that with
# the MaxDerivative value.  Derivatives not provided are computed via
# finite differences by GAMS
MaxDerivative = 0

# The one-argument version of Sine has the property that Sine(0) = 0.
ZeroRipple = 1


[Pi]
# For the constant function Pi, we apply almost all defaults, that is,
# the function has no arguments (and thus a continuous derivative) and
# can be used in equations.
# We only provide a description, as we would otherwise get an empty string.
# 
# However, we cannot omit the section itself, as we have to inform the
# ql.py script that the function 'Pi' exists.

Description = The ratio of the circumference of a circle to its diameter.
