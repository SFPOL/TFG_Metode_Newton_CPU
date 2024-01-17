#include "ShadingFactory.hh"


shared_ptr<ShadingStrategy> ShadingFactory::createShading(SHADING_TYPES t) {
    shared_ptr<ShadingStrategy> s;
    switch(t) {
    case COLOR:
        s= make_shared<ColorShading>();
        break;
    case COLORSHADOW:
        s = make_shared<ColorShadow>();
        break;
    case NORMAL:
        s = make_shared<NormalShading>();
        break;
    case NORMALSHADOW:
        s = make_shared<NormalShadow>();
        break;
    case DEPTH:
        s = make_shared<DepthShading>();
        break;
    case DEPTHSHADOW:
        s = make_shared<DepthShadow>();
        break;
    case CELL:
        s = make_shared<CellShading>();
        break;
    case CELLSHADOW:
        s = make_shared<CellShadow>();
        break;
    case BLINNPHONG:
        s = make_shared<BlinnPhongShading>();
        break;
    case BLINNPHONGSHADOW:
        s = make_shared<BlinnPhongShadow>();
        break;
    case PHONG:
        s = make_shared<PhongShading>();
        break;
    case PHONGSHADOW:
        s = make_shared<PhongShadow>();
        break;
    default:
        s = nullptr;
    }
    return s;
}

ShadingFactory::SHADING_TYPES ShadingFactory::getShadingType(QString name) {
    if (name=="COLOR") return SHADING_TYPES::COLOR;
    else if (name == "COLORSHADOW") return SHADING_TYPES::COLORSHADOW;
    else if (name == "NORMAL") return SHADING_TYPES::NORMAL;
    else if (name == "NORMALSHADOW") return SHADING_TYPES::NORMALSHADOW;
    else if (name == "DEPTH") return SHADING_TYPES::DEPTH;
    else if (name == "DEPTHSHADOW") return SHADING_TYPES::DEPTHSHADOW;
    else if (name == "CELL") return SHADING_TYPES::CELL;
    else if (name == "CELLSHADOW") return SHADING_TYPES::CELLSHADOW;
    else if (name == "BLINNPHONG") return SHADING_TYPES::BLINNPHONG;
    else if (name == "BLINNPHONGSHADOW") return SHADING_TYPES::BLINNPHONGSHADOW;
    else if (name == "PHONG") return SHADING_TYPES::PHONG;
    else if (name == "PHONGSHADOW") return SHADING_TYPES::PHONGSHADOW;
}

QString ShadingFactory::getNameType(SHADING_TYPES t) {
    switch (t) {
    case COLOR:
        return (QString("COLOR"));
        break;
    case COLORSHADOW:
        return (QString("COLORSHADOW"));
        break;
    case NORMAL:
        return (QString("NORMAL"));
        break;
    case NORMALSHADOW:
        return (QString("NORMALSHADOW"));
        break;
    case DEPTH:
        return (QString("DEPTH"));
        break;
    case DEPTHSHADOW:
        return (QString("DEPTHSHADOW"));
        break;
    case CELL:
        return (QString("CELL"));
        break;
    case CELLSHADOW:
        return (QString("CELLSHADOW"));
        break;
    case BLINNPHONG:
        return (QString("BLINNPHONG"));
        break;
    case BLINNPHONGSHADOW:
        return (QString("BLINNPHONGSHADOW"));
        break;
    case PHONG:
        return (QString("PHONG"));
        break;
    case PHONGSHADOW:
        return (QString("PHONGSHADOW"));
        break;
    default:
        return(QString(""));
    }
}
ShadingFactory::SHADING_TYPES ShadingFactory::getIndexType(shared_ptr<ShadingStrategy> m) {
    if (dynamic_pointer_cast<ColorShading>(m) != nullptr) {
        return SHADING_TYPES::COLOR;
    } else if (dynamic_pointer_cast<ColorShadow>(m) != nullptr) {
        return SHADING_TYPES::COLORSHADOW;
    } else if (dynamic_pointer_cast<NormalShading>(m) != nullptr) {
        return SHADING_TYPES::NORMAL;
    } else if (dynamic_pointer_cast<NormalShadow>(m) != nullptr) {
        return SHADING_TYPES::NORMALSHADOW;
    } else if (dynamic_pointer_cast<DepthShading>(m) != nullptr) {
        return SHADING_TYPES::DEPTH;
    } else if (dynamic_pointer_cast<DepthShadow>(m) != nullptr) {
        return SHADING_TYPES::DEPTHSHADOW;
    } else if (dynamic_pointer_cast<CellShading>(m) != nullptr) {
        return SHADING_TYPES::CELL;
    } else if (dynamic_pointer_cast<CellShadow>(m) != nullptr) {
        return SHADING_TYPES::CELLSHADOW;
    } else if (dynamic_pointer_cast<BlinnPhongShading>(m) != nullptr){
        return SHADING_TYPES::BLINNPHONG;
    } else if (dynamic_pointer_cast<BlinnPhongShadow>(m) != nullptr) {
        return SHADING_TYPES::BLINNPHONGSHADOW;
    } else if (dynamic_pointer_cast<PhongShading>(m) != nullptr){
        return SHADING_TYPES::PHONG;
    } else if (dynamic_pointer_cast<PhongShadow>(m) != nullptr){
        return SHADING_TYPES::PHONGSHADOW;
    } else
        return SHADING_TYPES::COLOR;
}

shared_ptr<ShadingStrategy> ShadingFactory::switchShading(shared_ptr<ShadingStrategy> m, bool shadow) {
    shared_ptr<ShadingStrategy> m_out = nullptr;
    if (shadow) {
        if (dynamic_pointer_cast<ColorShading>(m) != nullptr) {
             m_out = createShading(COLORSHADOW);
        }
        else if (dynamic_pointer_cast<BlinnPhongShading>(m) != nullptr) {
             m_out = createShading(BLINNPHONGSHADOW);
        }
        else if (dynamic_pointer_cast<CellShading>(m) != nullptr) {
             m_out = createShading(CELLSHADOW);
        }
        else if (dynamic_pointer_cast<NormalShading>(m) != nullptr) {
             m_out = createShading(NORMALSHADOW);
        }
        else if (dynamic_pointer_cast<PhongShading>(m) != nullptr) {
             m_out = createShading(PHONGSHADOW);
        }
        else if (dynamic_pointer_cast<DepthShading>(m) != nullptr) {
             m_out = createShading(DEPTHSHADOW);
        }
    } else {
        if (dynamic_pointer_cast<ColorShadow>(m) != nullptr) {
             m_out = createShading(COLOR);
        }
        else if (dynamic_pointer_cast<BlinnPhongShadow>(m) != nullptr) {
             m_out = createShading(BLINNPHONG);
        }
        else if (dynamic_pointer_cast<CellShadow>(m) != nullptr) {
             m_out = createShading(CELL);
        }
        else if (dynamic_pointer_cast<NormalShadow>(m) != nullptr) {
             m_out = createShading(NORMAL);
        }
        else if (dynamic_pointer_cast<PhongShadow>(m) != nullptr) {
             m_out = createShading(PHONG);
        }
        else if (dynamic_pointer_cast<DepthShadow>(m) != nullptr) {
             m_out = createShading(DEPTH);
        }
    }
    return m_out;
}
