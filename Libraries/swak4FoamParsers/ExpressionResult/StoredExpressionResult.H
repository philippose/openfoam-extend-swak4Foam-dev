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
    Foam::StoredExpressionResult

Description

SourceFiles
    StoredExpressionResultI.H
    StoredExpressionResult.C
    StoredExpressionResultIO.C

Contributors/Copyright:
    2012-2014 Bernhard F.W. Gschaider <bgschaid@hfd-research.com>

 SWAK Revision: $Id$
\*---------------------------------------------------------------------------*/

#ifndef StoredExpressionResult_H
#define StoredExpressionResult_H

#include "ExpressionResult.H"
#include "exprString.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class StoredExpressionResult Declaration
\*---------------------------------------------------------------------------*/

class StoredExpressionResult
:
    public ExpressionResult
{
    // Private data

    word name_;

    exprString initialValueExpression_;

public:

     //- Runtime type information
    TypeName("StoredExpressionResult");

    // Constructors

    //- Construct null
    StoredExpressionResult();

    //- Construct as copy
    StoredExpressionResult(const StoredExpressionResult&);

    //- Construct from a dictionary
    StoredExpressionResult(const dictionary&);

    // Destructor

    virtual ~StoredExpressionResult();

    void operator=(const StoredExpressionResult&);

    void operator=(const ExpressionResult&);

    virtual autoPtr<ExpressionResult> clone() const {
        return autoPtr<ExpressionResult>(
            new StoredExpressionResult(*this)
        );
    }

    const word name() const { return name_; }

    const exprString &initialValueExpression() const
        {
            return initialValueExpression_;
        }

    friend Ostream & operator<<(Ostream &,const StoredExpressionResult &);

    friend Istream & operator>>(Istream &,StoredExpressionResult &);

    friend bool operator!=(
        const StoredExpressionResult &,const StoredExpressionResult &
    );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "StoredExpressionResultI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
