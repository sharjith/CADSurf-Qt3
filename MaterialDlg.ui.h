/****************************************************************************
 ** ui.h extension file, included from the uic-generated form implementation.
 **
 ** If you want to add, delete, or rename functions or slots, use
 ** Qt Designer to update this file, preserving your code.
 **
 ** You should not define a constructor or destructor in this file.
 ** Instead, write your code in functions called init() and destroy().
 ** These will automatically be called by the form's constructor and
 ** destructor.
 *****************************************************************************/


void MatlEditor::slotGold()
{
    theMaterial.SetName(GOLD);
    updateSliders();
}


void MatlEditor::slotBrass()
{
    theMaterial.SetName(BRASS);
    updateSliders();
}


void MatlEditor::slotBronze()
{
    theMaterial.SetName(BRONZE);
    updateSliders();
}


void MatlEditor::slotCopper()
{
    theMaterial.SetName(COPPER);
    updateSliders();
}


void MatlEditor::slotSilver()
{
    theMaterial.SetName(SILVER);
    updateSliders();
}


void MatlEditor::slotJade()
{
    theMaterial.SetName(JADE);
    updateSliders();
}


void MatlEditor::slotPerl()
{
    theMaterial.SetName(PEARL);
    updateSliders();
}


void MatlEditor::slotPlastic()
{
    theMaterial.SetName(PLASTIC);
    updateSliders();
}


void MatlEditor::slotRuby()
{
    theMaterial.SetName(RUBY);
    updateSliders();
}


void MatlEditor::slotTurquoise()
{
    theMaterial.SetName(TURQUOISE);
    updateSliders();
}


void MatlEditor::init()
{
    theMaterial = CGLMaterial(DEFAULT);
    updateSliders();
}


void MatlEditor::slotDefaults()
{
    theMaterial.SetName(DEFAULT);
    updateSliders();
}


CGLMaterial MatlEditor::GetMaterial() const
{
    return theMaterial;
}


void MatlEditor::slotAmbientR()
{
    CGLColor amb(((float)sliderMAR->value()/(float)255),
	    ((float)sliderMAG->value()/(float)255),
	    ((float)sliderMAB->value()/(float)255));
    CGLColor dif(((float)sliderMDR->value()/(float)255),
	    ((float)sliderMDG->value()/(float)255),
	    ((float)sliderMDB->value()/(float)255));
    CGLColor spc(((float)sliderMSR->value()/(float)255),
	    ((float)sliderMDG->value()/(float)255),
	    ((float)sliderMSB->value()/(float)255));
    theMaterial.SetAmbient(amb);
    theMaterial.SetDiffuse(dif);
    theMaterial.SetSpecular(spc);
}


void MatlEditor::slotAmbientG()
{
    CGLColor amb(((float)sliderMAR->value()/(float)255),
	    ((float)sliderMAG->value()/(float)255),
	    ((float)sliderMAB->value()/(float)255));
    CGLColor dif(((float)sliderMDR->value()/(float)255),
	    ((float)sliderMDG->value()/(float)255),
	    ((float)sliderMDB->value()/(float)255));
    CGLColor spc(((float)sliderMSR->value()/(float)255),
	    ((float)sliderMDG->value()/(float)255),
	    ((float)sliderMSB->value()/(float)255));
    theMaterial.SetAmbient(amb);
    theMaterial.SetDiffuse(dif);
    theMaterial.SetSpecular(spc);
}


void MatlEditor::slotAmbientB()
{
    CGLColor amb(((float)sliderMAR->value()/(float)255),
	    ((float)sliderMAG->value()/(float)255),
	    ((float)sliderMAB->value()/(float)255));
    CGLColor dif(((float)sliderMDR->value()/(float)255),
	    ((float)sliderMDG->value()/(float)255),
	    ((float)sliderMDB->value()/(float)255));
    CGLColor spc(((float)sliderMSR->value()/(float)255),
	    ((float)sliderMDG->value()/(float)255),
	    ((float)sliderMSB->value()/(float)255));
    theMaterial.SetAmbient(amb);
    theMaterial.SetDiffuse(dif);
    theMaterial.SetSpecular(spc);
}


void MatlEditor::slotDiffuseR()
{
    CGLColor amb(((float)sliderMAR->value()/(float)255),
	    ((float)sliderMAG->value()/(float)255),
	    ((float)sliderMAB->value()/(float)255));
    CGLColor dif(((float)sliderMDR->value()/(float)255),
	    ((float)sliderMDG->value()/(float)255),
	    ((float)sliderMDB->value()/(float)255));
    CGLColor spc(((float)sliderMSR->value()/(float)255),
	    ((float)sliderMDG->value()/(float)255),
	    ((float)sliderMSB->value()/(float)255));
    theMaterial.SetAmbient(amb);
    theMaterial.SetDiffuse(dif);
    theMaterial.SetSpecular(spc);
}


void MatlEditor::slotDiffuseG()
{
    CGLColor amb(((float)sliderMAR->value()/(float)255),
	    ((float)sliderMAG->value()/(float)255),
	    ((float)sliderMAB->value()/(float)255));
    CGLColor dif(((float)sliderMDR->value()/(float)255),
	    ((float)sliderMDG->value()/(float)255),
	    ((float)sliderMDB->value()/(float)255));
    CGLColor spc(((float)sliderMSR->value()/(float)255),
	    ((float)sliderMDG->value()/(float)255),
	    ((float)sliderMSB->value()/(float)255));
    theMaterial.SetAmbient(amb);
    theMaterial.SetDiffuse(dif);
    theMaterial.SetSpecular(spc);
}


void MatlEditor::slotDiffuseB()
{
    CGLColor amb(((float)sliderMAR->value()/(float)255),
	    ((float)sliderMAG->value()/(float)255),
	    ((float)sliderMAB->value()/(float)255));
    CGLColor dif(((float)sliderMDR->value()/(float)255),
	    ((float)sliderMDG->value()/(float)255),
	    ((float)sliderMDB->value()/(float)255));
    CGLColor spc(((float)sliderMSR->value()/(float)255),
	    ((float)sliderMDG->value()/(float)255),
	    ((float)sliderMSB->value()/(float)255));
    theMaterial.SetAmbient(amb);
    theMaterial.SetDiffuse(dif);
    theMaterial.SetSpecular(spc);
}


void MatlEditor::slotSpecularR()
{
    CGLColor amb(((float)sliderMAR->value()/(float)255),
	    ((float)sliderMAG->value()/(float)255),
	    ((float)sliderMAB->value()/(float)255));
    CGLColor dif(((float)sliderMDR->value()/(float)255),
	    ((float)sliderMDG->value()/(float)255),
	    ((float)sliderMDB->value()/(float)255));
    CGLColor spc(((float)sliderMSR->value()/(float)255),
	    ((float)sliderMDG->value()/(float)255),
	    ((float)sliderMSB->value()/(float)255));
    theMaterial.SetAmbient(amb);
    theMaterial.SetDiffuse(dif);
    theMaterial.SetSpecular(spc);
}


void MatlEditor::slotSpecularG()
{
    CGLColor amb(((float)sliderMAR->value()/(float)255),
	    ((float)sliderMAG->value()/(float)255),
	    ((float)sliderMAB->value()/(float)255));
    CGLColor dif(((float)sliderMDR->value()/(float)255),
	    ((float)sliderMDG->value()/(float)255),
	    ((float)sliderMDB->value()/(float)255));
    CGLColor spc(((float)sliderMSR->value()/(float)255),
	    ((float)sliderMDG->value()/(float)255),
	    ((float)sliderMSB->value()/(float)255));
    theMaterial.SetAmbient(amb);
    theMaterial.SetDiffuse(dif);
    theMaterial.SetSpecular(spc);
}

void MatlEditor::slotSpecularB()
{
    CGLColor amb(((float)sliderMAR->value()/(float)255),
	    ((float)sliderMAG->value()/(float)255),
	    ((float)sliderMAB->value()/(float)255));
    CGLColor dif(((float)sliderMDR->value()/(float)255),
	    ((float)sliderMDG->value()/(float)255),
	    ((float)sliderMDB->value()/(float)255));
    CGLColor spc(((float)sliderMSR->value()/(float)255),
	    ((float)sliderMDG->value()/(float)255),
	    ((float)sliderMSB->value()/(float)255));
    theMaterial.SetAmbient(amb);
    theMaterial.SetDiffuse(dif);
    theMaterial.SetSpecular(spc);
}


void MatlEditor::slotShininess()
{
    theMaterial.SetShininessCoeff(((float)sliderShine->value()/(float)128));
}



void MatlEditor::updateSliders()
{
    sliderMAR->setValue((int)(theMaterial.GetAmbient().GetRed()*255));
    sliderMAG->setValue((int)(theMaterial.GetAmbient().GetGreen()*255));
    sliderMAB->setValue((int)(theMaterial.GetAmbient().GetBlue()*255));
    sliderMDR->setValue((int)(theMaterial.GetDiffuse().GetRed()*255));
    sliderMDG->setValue((int)(theMaterial.GetDiffuse().GetGreen()*255));
    sliderMDB->setValue((int)(theMaterial.GetDiffuse().GetBlue()*255));
    sliderMSR->setValue((int)(theMaterial.GetSpecular().GetRed()*255));
    sliderMSG->setValue((int)(theMaterial.GetSpecular().GetGreen()*255));
    sliderMSB->setValue((int)(theMaterial.GetSpecular().GetBlue()*255));
    sliderShine->setValue((int)(theMaterial.GetShininessCoeff()));
}


void MatlEditor::slotHelp()
{
 qWarning("Not.....");
}
