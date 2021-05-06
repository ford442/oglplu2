///
/// Copyright Matus Chochlik.
/// Distributed under the GNU GENERAL PUBLIC LICENSE version 3.
/// See http://www.gnu.org/licenses/gpl-3.0.txt
///

#include "TilingBackend.hpp"
#include "TilingModel.hpp"
//------------------------------------------------------------------------------
TilingBackend::TilingBackend(eagine::main_ctx_parent parent)
  : QObject{nullptr}
  , eagine::main_ctx_object{EAGINE_ID(Backend), parent}
  , _tilingModel{std::make_shared<TilingModel>(*this)}
  , _tilingTheme{*this}
  , _tilingViewModel{*this} {
    _timerId = startTimer(20);
    emit tilingModelChanged();
}
//------------------------------------------------------------------------------
TilingBackend::~TilingBackend() {
    killTimer(_timerId);
}
//------------------------------------------------------------------------------
void TilingBackend::timerEvent(QTimerEvent*) {
    if(_tilingModel) {
        _tilingModel->update();
    }
}
//------------------------------------------------------------------------------
auto TilingBackend::getTilingModel() noexcept -> TilingModel* {
    return _tilingModel.get();
}
//------------------------------------------------------------------------------
auto TilingBackend::getTilingTheme() noexcept -> TilingTheme* {
    return &_tilingTheme;
}
//------------------------------------------------------------------------------
auto TilingBackend::getTilingViewModel() noexcept -> TilingViewModel* {
    return &_tilingViewModel;
}
//------------------------------------------------------------------------------
