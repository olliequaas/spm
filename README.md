# spm
SPM (Simple Package Manager) is a simple package manager I made.
# Features
 - Syntax is similar to apt with install, reinstall, uninstall, update, search and QoL features like -y
 - Use @ to denote the version you want SPM to install, if no version is specified then it installs the default version. For example, chromium@1.0
 - Automatic dependency resolving
 - The `upgrade` operand has 3 flags, -a to upgrade every package, -s to upgrade/downgrade specified packages to a different version than currently installed and -n to upgrade specified packages to the latest available version
 - Auto updates man for each package installed if needed
 - Uses aria2 for ultra-fast download
