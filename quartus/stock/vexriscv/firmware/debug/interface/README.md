For unknown reasons the OpenOCD install does not always seem to compile with
default locations for config files.

As a result we are manually copying the required interface files from
`${OPENOCD_HOME}/tcl/interface/*`, where `${OPENOCD_HOME}/src` is the
directory added to PATH.