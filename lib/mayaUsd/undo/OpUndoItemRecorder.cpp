//
// Copyright 2021 Autodesk
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

#include <mayaUsd/undo/OpUndoItemRecorder.h>

namespace MAYAUSD_NS_DEF {

MAYAUSD_VERIFY_CLASS_NOT_MOVE_OR_COPY(OpUndoItemRecorder);

OpUndoItemRecorder::OpUndoItemRecorder(OpUndoItemList& undoInfo)
    : _undoInfo(undoInfo)
{
    // Clear anys item that may have been left behind in the given list.
    // and in the global container.
    _undoInfo.clear();
    OpUndoItemList::instance().clear();
}

OpUndoItemRecorder::~OpUndoItemRecorder()
{
    // Extract the undo items from the global container
    // into the container we were given.
    _undoInfo = std::move(OpUndoItemList::instance());
    OpUndoItemList::instance().clear();
}

} // namespace MAYAUSD_NS_DEF
