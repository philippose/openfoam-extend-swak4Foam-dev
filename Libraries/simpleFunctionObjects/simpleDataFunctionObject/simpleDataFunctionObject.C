/*---------------------------------------------------------------------------*\
 ##   ####  ######     |
 ##  ##     ##         | Copyright: ICE Stroemungsfoschungs GmbH
 ##  ##     ####       |
 ##  ##     ##         | http://www.ice-sf.at
 ##   ####  ######     |
-------------------------------------------------------------------------------
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright  held by original author
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is based on OpenFOAM.

    OpenFOAM is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

Contributors/Copyright:
    2008-2011, 2013, 2015 Bernhard F.W. Gschaider <bgschaid@hfd-research.com>

 SWAK Revision: $Id$
\*---------------------------------------------------------------------------*/

#include "simpleDataFunctionObject.H"
#include "addToRunTimeSelectionTable.H"

#include "polyMesh.H"
#include "IOmanip.H"
#include "swakTime.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(simpleDataFunctionObject, 0);

fileName simpleDataFunctionObject::defaultPostProcDir_("postProcessing");

void simpleDataFunctionObject::setPostProcDir(const fileName &f)
{
    Info << "Setting output directory name for simpleFunctionObjects to "
        << f << endl;
    defaultPostProcDir_=f;
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

simpleDataFunctionObject::simpleDataFunctionObject
(
    const word& name,
    const Time& t,
    const dictionary& dict
)
:
    simpleFunctionObject(name,t,dict),
    postProcDir_(defaultPostProcDir_)
{
    if(dict.found("postProcDir")) {
        postProcDir_=fileName(
            dict.lookup("postProcDir")
        );
        Info << name << " writes to " << postProcDir_
            << " instead of " << defaultPostProcDir_ << endl;
    }
}

fileName simpleDataFunctionObject::dataDir()
{
    return baseDir()/obr_.time().timeName();
}

fileName simpleDataFunctionObject::baseDir()
{
    fileName theDir;
    fileName dir=dirName()+"_"+name();

    if (Pstream::parRun())
    {
        // Put in undecomposed case (Note: gives problems for
        // distributed data running)
        theDir =
            obr_.time().path()
            /".."
            /postProcDir_
            /dir;
    }
    else
    {
        theDir =
            obr_.time().path()
            /postProcDir_
            /dir;
    }

    return theDir;
}

bool simpleDataFunctionObject::start()
{
    simpleFunctionObject::start();

    mkDir(dataDir());

    return true;
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

} // namespace Foam

// ************************************************************************* //
