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

Contributors/Copyright:
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

 SWAK Revision: $Id$
\*---------------------------------------------------------------------------*/

#include "swakPsiChemistryModelPluginFunction.H"
#include "FieldValueExpressionDriver.H"

#include "HashPtrTable.H"
#include "basicPsiThermo.H"
#include "basicRhoThermo.H"

#include "addToRunTimeSelectionTable.H"

namespace Foam {

defineTypeNameAndDebug(swakPsiChemistryModelPluginFunction,0);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

swakPsiChemistryModelPluginFunction::swakPsiChemistryModelPluginFunction(
    const FieldValueExpressionDriver &parentDriver,
    const word &name,
    const word &returnValueType,
    const string &spec
):
    FieldValuePluginFunction(
        parentDriver,
        name,
        returnValueType,
        spec
    )
{
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const psiChemistryModel &swakPsiChemistryModelPluginFunction::chemistryInternal(
    const fvMesh &reg
)
{
    static HashPtrTable<psiChemistryModel> chemistry_;

    if(reg.foundObject<psiChemistryModel>("chemistryProperties")) {
        if(debug) {
            Info << "swakPsiChemistryModelPluginFunction::chemistryInternal: "
                << "already in memory" << endl;
        }
        // Somebody else already registered this
        return reg.lookupObject<psiChemistryModel>("chemistryProperties");
    }
    if(!chemistry_.found(reg.name())) {
        if(debug) {
            Info << "swakPsiChemistryModelPluginFunction::chemistryInternal: "
                << "not yet in memory for " << reg.name() << endl;
        }

        // Create it ourself because nobody registered it
        chemistry_.set(
                reg.name(),
                psiChemistryModel::New(reg).ptr()
        );

        Info << "Created chemistry model. Calculating to get values ..."
            << endl;
        chemistry_[reg.name()]->solve(
            reg.time().value(),
            reg.time().deltaT().value()
        );
        //        chemistry_[reg.name()]->calculate();
    }

    return *(chemistry_[reg.name()]);
}

const psiChemistryModel &swakPsiChemistryModelPluginFunction::chemistry()
{
    return chemistryInternal(mesh());
}

// * * * * * * * * * * * * * * * Concrete implementations * * * * * * * * * //

#define concreteChemistryFunction(funcName,resultType)                \
class swakPsiChemistryModelPluginFunction_ ## funcName                \
: public swakPsiChemistryModelPluginFunction                          \
{                                                                  \
public:                                                            \
    TypeName("swakPsiChemistryModelPluginFunction_" #funcName);       \
    swakPsiChemistryModelPluginFunction_ ## funcName (                \
        const FieldValueExpressionDriver &parentDriver,            \
        const word &name                                           \
    ): swakPsiChemistryModelPluginFunction(                           \
        parentDriver,                                              \
        name,                                                      \
        #resultType                                                \
    ) {}                                                           \
    void doEvaluation() {                                          \
        result().setObjectResult(                                  \
            autoPtr<resultType>(                                   \
                new resultType(                                    \
                    chemistry().funcName()                            \
                )                                                  \
            )                                                      \
        );                                                         \
    }                                                              \
};                                                                 \
defineTypeNameAndDebug(swakPsiChemistryModelPluginFunction_ ## funcName,0);  \
addNamedToRunTimeSelectionTable(FieldValuePluginFunction,swakPsiChemistryModelPluginFunction_ ## funcName,name,psiChem_ ## funcName);

concreteChemistryFunction(tc,volScalarField);
concreteChemistryFunction(Sh,volScalarField);
concreteChemistryFunction(dQ,volScalarField);

class swakPsiChemistryModelPluginFunction_RR
: public swakPsiChemistryModelPluginFunction
{
    word speciesName_;

public:
    TypeName("swakPsiChemistryModelPluginFunction_RR");
    swakPsiChemistryModelPluginFunction_RR (
        const FieldValueExpressionDriver &parentDriver,
        const word &name
    ): swakPsiChemistryModelPluginFunction(
        parentDriver,
        name,
        "volScalarField",
        "speciesName primitive word"
    ) {}

    void doEvaluation() {
        label specI=chemistry().thermo().composition().species()[speciesName_];

        result().setObjectResult(
            autoPtr<volScalarField>(
                new volScalarField(
                    chemistry().RR(specI)
                )
            )
        );
    }

    void setArgument(label index,const word &s) {
        assert(index==0);

        speciesName_=s;
    }
};
defineTypeNameAndDebug(swakPsiChemistryModelPluginFunction_RR,0);
addNamedToRunTimeSelectionTable(FieldValuePluginFunction,swakPsiChemistryModelPluginFunction_RR,name,psiChem_RR);

class swakPsiChemistryModelPluginFunction_deltaTChem
: public swakPsiChemistryModelPluginFunction
{
public:
    TypeName("swakPsiChemistryModelPluginFunction_deltaTChem");
    swakPsiChemistryModelPluginFunction_deltaTChem (
        const FieldValueExpressionDriver &parentDriver,
        const word &name
    ): swakPsiChemistryModelPluginFunction(
        parentDriver,
        name,
        "volScalarField"
    ) {}

    void doEvaluation() {
        const DimensionedField<scalar,volMesh> &dtChem=chemistry().deltaTChem();

        autoPtr<volScalarField> val(
            new volScalarField(
                IOobject(
                    "deltaTChem",
                    mesh().time().timeName(),
                    mesh(),
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                mesh(),
                dimensionedScalar("dtChem",dtChem.dimensions(),0),
                "zeroGradient"
            )
        );
        val->dimensionedInternalField()=dtChem;

        result().setObjectResult(
            val
        );
    }
};
defineTypeNameAndDebug(swakPsiChemistryModelPluginFunction_deltaTChem,0);
addNamedToRunTimeSelectionTable(FieldValuePluginFunction,swakPsiChemistryModelPluginFunction_deltaTChem,name,psiChem_deltaTChem);

} // namespace

// ************************************************************************* //