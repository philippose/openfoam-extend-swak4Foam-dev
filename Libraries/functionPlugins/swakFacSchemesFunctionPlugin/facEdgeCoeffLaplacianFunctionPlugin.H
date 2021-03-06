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
    \\  /    A nd           | Copyright (C) 1991-2008 OpenCFD Ltd.
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

Class
    Foam::facEdgeCoeffLaplacianFunctionPlugin

Description

SourceFiles
    facEdgeCoeffLaplacianFunctionPlugin.C

Contributors/Copyright:
    2012-2013 Bernhard F.W. Gschaider <bgschaid@hfd-research.com>

 SWAK Revision: $Id$
\*---------------------------------------------------------------------------*/

#ifndef facEdgeCoeffLaplacianFunctionPlugin_H
#define facEdgeCoeffLaplacianFunctionPlugin_H

#include "FaFieldValuePluginFunction.H"

#include "runTimeSelectionTables.H"

#include "fac.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class facEdgeCoeffLaplacianFunctionPlugin Declaration
\*---------------------------------------------------------------------------*/

template<class T,class GT>
class facEdgeCoeffLaplacianFunctionPlugin
:
    public FaFieldValuePluginFunction
{
    //- Disallow default bitwise assignment
    void operator=(const facEdgeCoeffLaplacianFunctionPlugin &);

    facEdgeCoeffLaplacianFunctionPlugin(const facEdgeCoeffLaplacianFunctionPlugin &);

    typedef GeometricField<T,faPatchField,areaMesh> originalType;
    typedef GeometricField<T,faPatchField,areaMesh> resultType;
    typedef GeometricField<GT,faePatchField,edgeMesh> coeffType;

    autoPtr<originalType> original_;
    autoPtr<coeffType> coeff_;

    string specString_;

public:
    facEdgeCoeffLaplacianFunctionPlugin(
        const FaFieldValueExpressionDriver &parentDriver,
        const word &name
    );

    virtual ~facEdgeCoeffLaplacianFunctionPlugin() {}

    TypeName("facEdgeCoeffLaplacianFunctionPlugin");

    void doEvaluation();

    void setArgument(
        label index,
        const string &content,
        const CommonValueExpressionDriver &driver
    );

    void setArgument(label index,const string &);

};



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
