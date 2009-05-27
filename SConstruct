#/***************************************************************************
# *   Copyright (C) 2008 by Bruno Mahe                                      *
# *                                                                         *
# *   This program is free software; you can redistribute it and/or modify  *
# *   it under the terms of the GNU General Public License as published by  *
# *   the Free Software Foundation; either version 2 of the License, or     *
# *   (at your option) any later version.                                   *
# *                                                                         *
# *   This program is distributed in the hope that it will be useful,       *
# *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
# *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
# *   GNU General Public License for more details.                          *
# *                                                                         *
# *   You should have received a copy of the GNU General Public License     *
# *   along with this program; if not, write to the                         *
# *   Free Software Foundation, Inc.,                                       *
# *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
# ***************************************************************************/



import os
import os.path
import sys
import shutil

import scons.BaseGnollInstaller
import SCons.Defaults

# Taken from scons Wiki
def builder_unit_test(target, source, env):
	app = str(source[0].abspath)
	if os.spawnl(os.P_WAIT, app, app) == 0:
		open(str(target[0]), 'w').write("PASSED\n")
	else:
		return 1

test_builder = Builder(action = builder_unit_test)

def checkDoxygen(env, config):
	return not env.Execute('doxygen --version')

def buildDoxygenDoc(env, config):
	env.Execute('doxygen')


def print_config(config):

	print
	print
	print "***************************************************************"
	print "* Generating Configuration files with the following settings: *"
	print "***************************************************************"
	print

	keys = config.keys()
	keys.sort()
	for key in keys:
		print "    %-30s %s" % (key, config[key])
	print
	print


def generateConfigFile(config, newSymbols, srcFile, destFile):

	print_config(config)

	for target, source in zip(destFile, srcFile):

		print "Generating [%s] from [%s]" % (target, source)

		config_h = file(str(target), "w")
		config_h_in = file(str(source), "r")

		config_h.write(config_h_in.read() % config)


		config_h.write("\n")
		config_h.write("\n")
		config_h.write("/* Define various constants */")
		config_h.write("\n")
		config_h.write("\n")


		symbols = newSymbols.keys()
		symbols.sort()

		for symbol in symbols:
			config_h.write("#define %s   %s\n" %(symbol, newSymbols[symbol]))
			config_h.write("\n")

		config_h_in.close()
		config_h.close()

		print


def constructGnollEnvironment(env, configProject, config):
	gnollBuilder = None

	if (env['PLATFORM'] == 'posix'):

		from  scons.LinuxGnollBuilder import LinuxGnollBuilder
		gnollBuilder = LinuxGnollBuilder()


	if (gnollBuilder == None):

		print "Sorry, your platform [%s] is not supported." % (env['PLATFORM'])
		exit(-1)

	# If debug is activated, add some flags to ad debugging symbols
	if configProject['DEBUG'] == '1':
		env.MergeFlags([ ' -g ', ' -pg '])
	else:
		env.MergeFlags([ ' -O3 '])

	# Check if all libraries needed by Gnoll are installed and configured
	gnollConfig = gnollBuilder.checkGnoll(env, config)

	# Generate config.h file
	generateConfigFile(configProject, gnollConfig, ['./src/config.h.in'], ['./src/config.h'])

	return gnollBuilder


def main():


	configProject = {
				"DEBUG"  : "0",
				"DO_LOG"  : "0",
				"DEFAULT_LOG_FILENAME"  : '"./gnoll.log"',
				"PACKAGE_NAME": '"Gnoll"',
				"PACKAGE_VERSION": '"0.1.7"' ,
				"PACKAGE_BUGREPORT": '"gnoll-devel@lists.gnoll.org"',
				"PACKAGE_STRING": '"Gnoll 0.1.7"',
				"PACKAGE_TARNAME": '"gnoll"'
			}


	opts = Options('gnoll.conf')
	opts.Add(
					PathOption(
							'prefix',
							'Directory to install under',
							'/usr/share',
							SCons.Options.PathOption.PathAccept
							)
				)


	# Do we want to compile in debug mode ?
	if ARGUMENTS.get('debug', '0') == '1':
		configProject['DEBUG'] = '1'

	# Do we want to activate logging ?
	if ARGUMENTS.get('do_log', '0') == '1':
		configProject['DO_LOG'] = '1'


	for target in BUILD_TARGETS:

		print "Target [%s]" % (target)
		env = Environment(ENV = os.environ)
		config = env.Configure()
		env.BuildDir("./objs/", "src")

		# Change default log path for posix systems
		if (env['PLATFORM'] == 'posix'):

			linuxLogDir = os.path.expanduser("~") + '/.gnoll/logs'
			configProject["DEFAULT_LOG_FILENAME"] = '"' + linuxLogDir + '/gnoll.log' + '"'


		env['install_prefix'] = ARGUMENTS.get('prefix')
		if not env['install_prefix']:
			env['install_prefix'] = "/usr/"

		if env['install_prefix'][-1] != '/' :
			env['install_prefix'] = env['install_prefix'] + '/'


		env['ogre_lib_path'] = ARGUMENTS.get('ogre_lib_path')
		if not env['ogre_lib_path']:
			env['ogre_lib_path'] = "/usr/lib/OGRE/"

		if env['ogre_lib_path'][-1] != '/' :
			env['ogre_lib_path'] = env['ogre_lib_path'] + '/'



		env['install_bin'] = env['install_prefix'] + "bin/"
		env['install_data'] = env['install_prefix'] + 'share/gnoll-example/data/'
		env['install_config'] = env['install_data']



		opts.Update(env)
		opts.Save('gnoll.conf', env)

		print "install_prefix = [%s]" % (env['install_prefix'])
		print "install_bin    = [%s]" % (env['install_bin'])
		print "install_data   = [%s]" % (env['install_data'])
		print "install_config = [%s]" % (env['install_config'])

		Help(opts.GenerateHelpText(env))




		if target == 'gnoll':
			gnollBuilder = constructGnollEnvironment(env, configProject, config)

			# Build Gnoll
			gnollBuilder.buildGnoll(env, config)

		# testcore will be replaced by a list of test targets
		if target == 'tests':
			env.Append(BUILDERS = {'Test' : test_builder })
			gnollBuilder = constructGnollEnvironment(env, configProject, config)

			# Build Gnoll
			gnollBuilder.buildTests(env, config)


		if target == 'doc':

			if not checkDoxygen(env, config):
				print "Doxygen utility not found."
				exit(-1)
			buildDoxygenDoc(env, config)


		if target == 'install':

			gnollInstaller = None

			# DEFAULT_LOG_FILENAME has the path enclosed by "
			# dirname will remove the ending " but not the very first one
			logDir = os.path.dirname( configProject['DEFAULT_LOG_FILENAME'] )
			logDir = logDir[1:]

			if not os.path.exists( logDir ):
				try:
					print "Creating destination logs directory [%s]" % (logDir)
					os.makedirs(logDir)
				except OSError, why:
					print "Can't create %s: %s" % (logDir, why)
			else:
				print "No need to create %s. It already exists" % (logDir)


			if (env['PLATFORM'] == 'posix'):

				from  scons.LinuxGnollInstaller import LinuxGnollInstaller
				gnollInstaller = LinuxGnollInstaller()


			# Step 1 : Copy static files from ./data/
			# This proceeds only if 'install_data' doesn't exists
			shutil.rmtree(env['install_data'])
			if not os.path.exists(env['install_data']):
				try:
					print "Copying data [Destination dir: %s]..." % (env['install_data'])
					shutil.copytree('./data/', env['install_data'], True)

				except (IOError, os.error), why:
					print "Can't copy %s to %s: %s" % ('./data/', `env['install_data']`, str(why))
			else:
				print "%s already exists. No need to install files from ./data/" % (`env['install_data']`)


			# If no specific installer has been found, use the base one
			if gnollInstaller == None:
				gnollInstaller = scons.BaseGnollInstaller.BaseGnollInstaller()


			# Define binaries	installation target
			env.Alias('install_bin_gnoll', env.Install(env['install_bin'], 'gnoll'))
			env.Alias('install_bin', ['install_bin_gnoll'])
			env.Alias('install', ['gnoll', 'install_bin'], )


			# Launche install process
			if (gnollInstaller != None):
				gnollInstaller.install(env, config)


		config.Finish()


main()
