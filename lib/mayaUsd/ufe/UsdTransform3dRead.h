//
// Copyright 2022 Autodesk
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#ifndef MAYAUSD_USDTRANSFORM3DREAD_H
#define MAYAUSD_USDTRANSFORM3DREAD_H

#include <mayaUsd/base/api.h>
#include <mayaUsd/ufe/UsdTransform3dReadImpl.h>

#include <usdUfe/ufe/UsdSceneItem.h>

#include <pxr/usd/usd/prim.h>

#include <ufe/transform3d.h>
#include <ufe/transform3dHandler.h>

namespace MAYAUSD_NS_DEF {
namespace ufe {

//! \brief Read-only implementation for USD object 3D transform information.
//
// Note that all calls to specify time use the default time, but this
// could be changed to use the current time, using getTime(path()).

class MAYAUSD_CORE_PUBLIC UsdTransform3dRead
    : private UsdTransform3dReadImpl
    , public Ufe::Transform3dRead
{
public:
    typedef std::shared_ptr<UsdTransform3dRead> Ptr;

    //! Create a UsdTransform3dRead.
    static UsdTransform3dRead::Ptr create(const UsdUfe::UsdSceneItem::Ptr& item);

    UsdTransform3dRead(const UsdUfe::UsdSceneItem::Ptr& item);

    MAYAUSD_DISALLOW_COPY_MOVE_AND_ASSIGNMENT(UsdTransform3dRead);

    // Ufe::Transform3d overrides
    const Ufe::Path&    path() const override;
    Ufe::SceneItem::Ptr sceneItem() const override;

    inline UsdUfe::UsdSceneItem::Ptr usdSceneItem() const
    {
        return UsdTransform3dReadImpl::usdSceneItem();
    }
    inline PXR_NS::UsdPrim prim() const { return UsdTransform3dReadImpl::prim(); }

    Ufe::Matrix4d matrix() const override;

    Ufe::Matrix4d segmentInclusiveMatrix() const override;
    Ufe::Matrix4d segmentExclusiveMatrix() const override;
}; // UsdTransform3dRead

//! \brief Factory to create a UsdTransform3dRead interface object.
class MAYAUSD_CORE_PUBLIC UsdTransform3dReadHandler : public Ufe::Transform3dHandler
{
public:
    using Ptr = std::shared_ptr<UsdTransform3dReadHandler>;

    UsdTransform3dReadHandler(const Ufe::Transform3dHandler::Ptr& nextHandler);

    MAYAUSD_DISALLOW_COPY_MOVE_AND_ASSIGNMENT(UsdTransform3dReadHandler);

    //! Create a UsdTransform3dReadHandler.
    static Ptr create(const Ufe::Transform3dHandler::Ptr& nextHandler);

    // Ufe::Transform3dHandler overrides
    Ufe::Transform3d::Ptr     transform3d(const Ufe::SceneItem::Ptr& item) const override;
    Ufe::Transform3dRead::Ptr transform3dRead(const Ufe::SceneItem::Ptr& item) const override;
    Ufe::Transform3d::Ptr     editTransform3d(
            const Ufe::SceneItem::Ptr&      item,
            const Ufe::EditTransform3dHint& hint = Ufe::EditTransform3dHint()) const override;

private:
    Ufe::Transform3dHandler::Ptr _nextHandler;

}; // UsdTransform3dReadHandler

} // namespace ufe
} // namespace MAYAUSD_NS_DEF

#endif // MAYAUSD_USDTRANSFORM3DREAD_H
